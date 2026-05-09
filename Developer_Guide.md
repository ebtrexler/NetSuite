# NETrex Developer Guide

> **Scope.** This guide is for developers who will maintain or extend NETrex.
> It covers (1) what the application does, (2) the biophysical models and the
> numerical integrator, (3) the real-time DAQ pipeline, and (4) rig profiles
> for cross-lab portability.
>
> End-user installation and build instructions live in `README.md`.
> End-user workflow documentation lives in `User_Guide.md`.

> **Status note.** Sections 3 and 4 of this document were originally written
> as a *proposal* for the threaded DAQ pipeline. That pipeline is now
> implemented. Read §3 as "what the old single-threaded code looked like"
> and §4 as "the as-built design." Section 8 (Rig profiles) was added
> after the pipeline shipped. Section 6 marks resolved open questions.

---

## 1. What the application does

NETrex is a real-time neural-network modeling and *dynamic-clamp* application
that merges the legacy NetSim (pure simulation), NetClamp (dynamic clamp), and
NetFit (parameter fitting) programs. A user constructs a **network** of cells,
currents, synapses, and electrodes through a Qt GUI, then either:

* **Simulates** the network numerically (no hardware involved), or
* Runs a **dynamic-clamp** experiment: membrane voltages are sampled from a
  real biological cell through an NI-DAQmx analog-input channel, fed into
  the network model, and the computed synaptic / intrinsic currents are
  written back to analog-output channels that drive an amplifier's
  current-injection input.

### 1.1 Component map

```
+-------------------------+     +-------------------------+
|  Qt UI (ui/)            |     |  Core library (core/)   |
|                         |     |                         |
|  MainWindow ──────────────────▶  TNetwork               |
|  NetworkEditor          |     |   ├─ TCellsMap          |
|  NetworkView            |     |   ├─ TSynapsesMap       |
|  RunDialog  ──────┐     |     |   ├─ TElectrodesMap     |
|  TracePanel       │     |     |   └─ TCurrentsMap       |
|   └─ TracePlot    │     |     |                         |
|      (RingBuffer) │     |     |  TCell                  |
|                   │     |     |   ├─ TModelCell (RK4)   |
|                   │     |     |   ├─ TBiologicalCell    |
|                   │     |     |   └─ TPlaybackCell      |
|                   │     |     |                         |
|                   ▼     |     |  TCurrent               |
|             DAQInterface──────▶   ├─ THHCurrent         |
|              ├─ DAQMock        |    (uses RK4 gates)    |
|              └─ DAQNIDAQmx     |   ├─ THH2Current       |
|                                |   ├─ TGJCurrent        |
|                                |   ├─ TVoltageClampPID  |
|                                |   └─ TPlaybackCurrent  |
+-------------------------+     +-------------------------+
```

Key files to know:

| Area | File(s) |
|---|---|
| Network / orchestration | `core/network/RT_Network.{h,cpp}` |
| Cell base + concrete cells | `core/network/RT_Cell.{h,cpp}`, `core/network/RT_ModelCell.h`, `core/network/RT_BiologicalCell.h`, `core/network/RT_PlaybackCell.h` |
| HH biophysics | `core/models/RT_HHCurrent.{h,cpp}`, `core/models/RT_HHKineticsFactor.h` |
| DAQ abstraction | `core/daq/daq_interface.h`, `core/daq/daq_nidaqmx.h`, `core/daq/daq_mock.h`, `core/daq/daq_enumerate.h` |
| DAQ real-time hot loop | `core/daq/daq_thread.{h,cpp}`, `core/daq/rt_priority.h` |
| Concurrency primitives | `core/math/spsc_ring.h` (UI side, drop-oldest), `core/math/spsc_queue.h` (disk side, guaranteed-delivery), `core/math/ring_buffer.h` (legacy UI-only) |
| Binary recording format | `core/io/scan_writer.{h,cpp}`, `core/io/ntrx_reader.h` (readers of `.ntrx` files) |
| Rig profiles | `core/io/rig_profile.h`, `ui/editors/rigprofiledialog.h` |
| Network JSON I/O | `core/io/network_json.h` |
| Main run loop | `ui/editors/rundialog.h` (`RunDialog::runStep` / `daqStep` drain UI rings at 30 Hz) |
| Trace plotting | `ui/visualizers/traceplot.{h,cpp}`, `ui/visualizers/tracepanel.{h,cpp}` |

**Test binaries** (all built from `apps/*.cpp`, link against `netsuite_core`):

| Binary | What it tests |
|---|---|
| `test_core` | Smoke test: build an HH network, initialize it. |
| `test_spsc` | 10 tests: ring + queue correctness, MT producer/consumer, simulated UI stall. |
| `test_scan_writer` | 7 tests: binary format round-trip, backpressure, crash recovery. |
| `test_daq_thread` | 3 end-to-end tests: DaqThread against DAQMock with real ScanWriter + on_chunk callback. |
| `test_rig_profile` | 10 tests: profile JSON I/O, manager state, role resolution vs. stored fallback. |
| `ntrx2csv` | CLI utility (not a test): streams `.ntrx` → CSV. |

### 1.2 Units

The entire core obeys one convention (documented in
`RT_HHKineticsFactor.h`):

* conductance — **µS**
* voltage — **mV**
* current — **nA**
* time — **ms**

Because µS × mV = nA, Ohm's law `I = G·(V − E)` falls out dimensionally clean.
DAQ values in `DAQInterface` are in **Volts** on the wire; they are multiplied
by `TCell::AIGain` (mV/V) on read and `TCell::AOGain` (V/nA) on write.

### 1.3 File format

Networks are saved/loaded as JSON (`core/io/network_json.h`, built on
`nlohmann::json`). A file contains arrays of cells, electrodes, synapses, and
their per-object currents.

---

## 2. Biophysics modeled

### 2.1 Cell model: integrate-and-fire with arbitrary currents

`TModelCell` (`core/network/RT_ModelCell.h`) is a single-compartment neuron.
Its state is a single membrane voltage **V_m (mV)** and a lumped membrane
capacitance **C (nF)** (default 10). Per integration step the cell solves

```
           dV_m     I_total
           ────  =  ─────────
            dt         C
```

where `I_total` (nA) is the algebraic sum of all intrinsic ionic currents,
synaptic currents, and electrode (injected) currents attached to the cell —
summed in `TCell::DoUpdate()`:

```cpp
F_I -= sum(intrinsic currents);   // ionic
F_I += sum(electrodes);           // injected
F_I -= sum(synaptic currents);    // synaptic
```

`TBiologicalCell` does **not** integrate V_m; instead its `SetVm()` is called
with a DAQ-sampled voltage each step and it emits the computed `I_nA` to the
DAC (with per-cell positive/negative current clamps).

`TPlaybackCell` plays back a pre-recorded voltage trace and is useful for
stimulating downstream network elements with a known waveform.

### 2.2 Hodgkin–Huxley currents

`THHCurrent` is the polymorphic workhorse. The formula follows the
Tohidi–Nadim / Willms compact form with Cataldo's extensions:

```
I = G_max · m(V)^p · h(V)^q · n(V)^r · (V_drv − E)
```

* `G_max` is the peak conductance (µS).
* `E` is the reversal potential (mV).
* `p, q, r` are integer exponents (any of them can be 0 to disable that
  gate).
* `V_drv` drives the ionic flow. For intrinsic currents it equals V_m;
  for a synapse, **V_kin = V_pre** (kinetics follow the presynaptic cell)
  while **V_drv = V_post** (ions flow with respect to the postsynaptic
  cell).

Each gating variable `y ∈ {m, h, n}` is governed by first-order kinetics:

```
dy(t,V)     y_inf(V) − y(t,V)
───────  =  ─────────────────
   dt            tau_y(V)
```

with Boltzmann-style voltage dependence (see `THHKineticsFactor::inf()` and
`::tau()`):

```
                1 − inf_min
inf(V) = ───────────────────────────── + inf_min
          [ 1 + exp((V − V0)/k) ]^w

                          t_lo − t_hi
tau(V) = t_hi + ─────────────────────────────
                 [ 1 + exp((V − V0)/|k|) ]^w
```

The per-kinetic-factor parameters `V0, k, t_lo, t_hi, inf_min, w` are editable
from the UI.

### 2.3 The Runge–Kutta integrator

NETrex uses the **classical 4th-order Runge–Kutta method (RK4)** to advance
every ODE in the model. Given `y_n` and a step `h`, RK4 computes

```
k1 = h · f(t_n,           y_n)
k2 = h · f(t_n + h/2,     y_n + k1/2)
k3 = h · f(t_n + h/2,     y_n + k2/2)
k4 = h · f(t_n + h,       y_n + k3)

y_{n+1} = y_n + (k1 + 2·k2 + 2·k3 + k4) / 6
```

RK4 is fourth-order accurate (local truncation error O(h⁵), global O(h⁴)).
It's a sweet spot for HH-type stiff-but-not-too-stiff neuronal equations:
much more stable than forward Euler and much cheaper per accepted step than
an implicit scheme.

Where RK4 shows up in the codebase:

1. **Gating variables.** `THHKineticsFactor::rk4()` integrates each
   `m, h, n` independently. `f(y) = (y_inf − y) / tau_y`. The kinetic
   factor's `V` is held **frozen over the step** (the step is short compared
   to any V-dynamics), so the ODE is effectively linear-in-y with
   parameters computed once per call.
2. **Membrane voltage.** `TModelCell::rk4()` integrates V_m with
   `f(V) = I_total / C`. For the duration of one DAQ step the total current
   is held constant (re-evaluated once per step), which preserves accuracy
   at typical sample rates (10–50 kHz) while keeping the inner loop cheap.
3. **Sub-stepping for stability.** Every `Update()` caller passes the wall-
   clock step `step_ms`. If `step_ms > TNetwork::GetMaxRK4Timestep()` (the
   user-configurable cap, default **0.01 ms**), `THHKineticsFactor::Update`
   automatically breaks the interval into `floor(step / max)` full sub-
   steps plus a final remainder step. This decouples the network's
   stability from the DAQ rate, so the user can acquire at 5 kHz and
   still integrate the kinetics at 100 kHz internally.
4. **Interpolation.** `RunDialog` exposes an *Interpolate* checkbox that
   forces extra sub-steps inside one DAQ scan (distinct from the RK4 cap).
   This is the mechanism by which NetClamp keeps the model well-integrated
   even when DAQ hardware only permits low AI rates.

### 2.4 Periodic modulation

Every current has an optional `TCurrent::PeriodGain()` that multiplies its
output by a square-pulse / sine / triangle envelope with user-configurable
period, duty cycle, initial delay, and triangle phase. This is implemented
in the base class so all current types benefit.

### 2.5 Electrodes, synapses, and other current/cell types

* **TInjectionElectrode** — current-clamp with pulse timing
  (amplitude/delay/duration/repeats).
* **TPlaybackElectrode** — plays back an arbitrary current waveform.
* **TPlaybackCurrent** — same but attached to a cell's intrinsic-currents
  list.
* **TGapJunctionSynapse / TGJCurrent** — ohmic coupling between two cells.
* **TGenBiDirSynapse** — generic bidirectional chemical synapse.
* **TVoltageClampPIDCurrent** — software voltage clamp implemented as a PID
  controller on the injected current (used to hold a cell at a target V_m).
* **THH2Current** — two-step HH with a second, slower recovery state
  (useful for e.g. K_slow or NaP).

---

## 3. Current DAQ architecture (as-is)

### 3.1 Flow diagram (what `RunDialog::daqStep` does today)

```
    ┌──────────── UI thread (the only thread) ────────────┐
    │                                                     │
    │  QTimer (1 ms)                                      │
    │     │                                               │
    │     ▼                                               │
    │  daqStep()                                          │
    │     ├─ m_daq->readAI(m_readBuf, N)          ◀──── blocks for up to 10 s
    │     ├─ m_net->Update(step, V_in, V_out, I)          │
    │     ├─ clamp I_nA to ±10 V, m_daq->writeAO(I_nA)    │
    │     ├─ tracePanel->addDataPoint(i, t, v)            │
    │     └─ *m_saveStream << t << "," << v << "\n"       │
    │                                                     │
    │  (paintEvent also runs on this same thread)         │
    └─────────────────────────────────────────────────────┘
```

### 3.2 Problems with the current implementation

1. **Single thread.** Acquisition, network integration, CSV writing, and
   repainting all share the Qt main thread. A slow repaint, a GC-style
   pause (macOS compositor hiccups, spotlight indexer, etc.), or a long
   synchronous file write will starve `readAI` and the NI-DAQmx driver's
   hardware FIFO will overrun.
2. **Not thread-safe.** `RingBuffer` has an explicit
   *"Not thread-safe — caller must synchronize"* disclaimer and is only
   used on the plotting side.
3. **Lossy disk writes.** `QTextStream` writing on the same thread as
   `readAI` is both slow (formats every value to ASCII) and lossy if the
   UI stalls before a scan is recorded.
4. **Plot tied to acquisition cadence.** If `paintEvent` stalls, we stop
   consuming DAQ samples — so a UI bog-down silently drops experimental
   data.

The rewrite below fixes all four.

---

## 4. Next steps: a real-time DAQ pipeline

### 4.1 Goals (rules of the road)

1. **Acquisition never waits for the UI.** A dedicated DAQ thread is the
   only thing that calls `readAI`/`writeAO`.
2. **No data is lost.** Every scan that comes off the hardware is written
   to disk by a **writer thread** whose only source is a lock-free queue
   fed by the DAQ thread.
3. **The UI can skip.** The plot reads from a ring buffer; if it falls
   behind (OS pause, Mission Control, slow screen), it catches up by
   replaying the most recent N samples on the next repaint. It never
   back-pressures the producer.
4. **Back-pressure is visible.** If the ring buffer is full (UI hasn't
   consumed in a while), we log an overflow counter — we do not drop disk
   writes to compensate.

### 4.2 Thread model

```
    ┌───────────────┐   samples   ┌───────────────┐   samples   ┌───────────────┐
    │  DAQ thread   │────────────▶│ DiskWriter    │             │   UI thread   │
    │  (REALTIME)   │    SPSC     │  thread       │             │   (Qt normal) │
    │               │   queue A   │  (binary f64) │             │               │
    │  readAI ────┐ │             └───────────────┘             │  paintEvent   │
    │  RK4 steps  │ │                                           │    ▲          │
    │  writeAO ───┤ │   samples   ┌───────────────┐   snapshot  │    │          │
    │             │─┼────────────▶│  RingBuffer   │◀────────────┘    │          │
    │             │ │   lock-free │  (overwrite   │                  │          │
    └─────────────┼─┘   SPSC B    │   oldest)     │                  │          │
                  │               └───────────────┘                  │          │
                  │                                                  │          │
                  └── QTimer(33 ms) ── emits "plot me" ──────────────┘          │
                                                                                ▼
                                                                     QWidget::update()
```

Three threads, two queues:

* **DAQ thread** (the only real-time actor): pulls samples from NI-DAQmx,
  drives `TNetwork::Update()` with the acquired V_m vector, writes current
  commands back, and pushes every scan into **both** queue A (to disk)
  and queue B (to the plotter).
* **Writer thread**: blocking read of queue A, writes binary float64 scans
  to a pre-allocated file. Flushes at a configurable cadence (e.g. 100 ms).
  Optionally transcodes to CSV at the end of the run (not in the hot path).
* **UI thread**: a `QTimer` (~30 Hz) snapshots queue B's latest window and
  repaints. Late repaints simply take a later snapshot — they never block
  the DAQ thread.

### 4.3 The SPSC ring buffer

Queue A and queue B are both bounded, single-producer / single-consumer
ring buffers with slightly different semantics:

| | Queue A (→ disk) | Queue B (→ plot) |
|---|---|---|
| Producer | DAQ thread | DAQ thread |
| Consumer | Writer thread | UI thread |
| On full | **block producer briefly** (disk stall → raise error) | **overwrite oldest** |
| Purpose | "no data is lost" | "UI catches up after OS pause" |

The existing `core/math/ring_buffer.h` is a good starting point for the
**UI side** (queue B). It already has the "overwrite oldest" semantics
(`push` advances `m_head` modulo capacity). What it needs before being
used cross-thread:

1. Make `m_head` and `m_size` `std::atomic<size_t>` and publish updates
   with `memory_order_release` from the producer, `memory_order_acquire`
   from the consumer.
2. Split the API: producer calls `push()`; consumer calls
   `snapshot_from(size_t &cursor, std::vector<T> &out)` which copies
   everything newer than `cursor` and updates `cursor`. The consumer never
   mutates the buffer itself.
3. Size it generously — **2 seconds' worth** of samples at the maximum
   supported rate is a safe default (at 50 kHz, 4 channels × 8 bytes × 2 s
   ≈ 3 MB).

Queue A needs different semantics and should be a **separate, bounded
SPSC queue** (block producer when full). A simple header-only choice:
`boost::lockfree::spsc_queue` or a hand-rolled 128-scan chunked queue
(chunked is better — write contiguous blocks to disk rather than scan-by-
scan).

### 4.4 DAQ thread design

```cpp
// core/daq/daq_thread.h  (new file, sketch)
class DAQThread {
public:
    DAQThread(DAQInterface *daq, TNetwork *net,
              ScanWriter *writer, ScanRingBuffer *uiRing);
    void start();   // launches std::thread, sets RT priority
    void stop();    // sets m_stop, joins
    bool overflowedUI() const { return m_uiOverflows.load(); }
    uint64_t scansAcquired() const { return m_scans.load(); }

private:
    void run();

    DAQInterface        *m_daq;
    TNetwork            *m_net;
    ScanWriter          *m_writer;
    ScanRingBuffer      *m_uiRing;
    std::thread          m_thread;
    std::atomic<bool>    m_stop{false};
    std::atomic<uint64_t> m_scans{0};
    std::atomic<uint64_t> m_uiOverflows{0};
};
```

The hot loop:

```cpp
void DAQThread::run() {
    setRealtimePriority();            // see 4.6
    std::vector<double> aiBuf(kChunkSamples * m_numAI);
    std::vector<double> aoBuf(m_numAO);
    std::vector<double> Vm_out(m_numCells);

    while (!m_stop.load(std::memory_order_relaxed)) {
        int32_t scans = m_daq->readAI(aiBuf.data(), aiBuf.size());
        if (scans <= 0) continue;

        // integrate the network with the last acquired V_m
        double *last = &aiBuf[(scans - 1) * m_numAI];
        m_net->Update(scans * m_step_ms, last, Vm_out.data(), aoBuf.data());

        // drive DAC — this must happen every cycle to keep the
        // dynamic-clamp loop closed
        m_daq->writeAO(aoBuf.data(), m_numAO);

        // disk: guaranteed-delivery queue, chunked write
        if (!m_writer->enqueue(aiBuf.data(), scans, Vm_out.data(), aoBuf.data()))
            m_stop.store(true);       // disk backed up → abort run cleanly

        // UI: drop-oldest queue, never blocks
        if (!m_uiRing->pushScan(aiBuf.data(), scans, Vm_out.data()))
            m_uiOverflows.fetch_add(1, std::memory_order_relaxed);

        m_scans.fetch_add(scans, std::memory_order_relaxed);
    }
}
```

Points worth highlighting:

* **One `Update()` per DAQ chunk, not per sample.** This matches today's
  behavior in `RunDialog::daqStep`. Pass `scans * m_step_ms` as the step;
  RK4 sub-stepping (see §2.3) keeps the kinetics well-integrated.
* **AO is driven every cycle**, even during the "Before" and "After"
  phases — either with the computed command or with zeros. A silent AO
  task can glitch on some NI cards.
* The DAQ thread does **no memory allocation** in the hot path. All
  buffers are sized in `start()`.

### 4.5 Disk writer thread

Two design choices matter:

1. **Write binary, transcode later.** `double` scans go to disk as raw
   little-endian float64 — ~3× faster than `QTextStream` and lossless.
   Convert to CSV in `finishRun()` if the user asked for CSV.
2. **Chunk on disk boundaries, not scan boundaries.** Batch 100 ms worth
   of scans per `pwrite`/`fwrite` call. Open the file with
   `O_APPEND` (POSIX) / `FILE_FLAG_SEQUENTIAL_SCAN` (Win32). On Linux,
   periodically call `fdatasync` from the writer thread (never from the
   DAQ thread).

Header format (suggested, little-endian):

```
  offset  size  field
  ------  ----  -----
     0      4   magic 'NTRX'
     4      4   version (uint32)
     8      8   sample_rate_hz (float64)
    16      4   num_ai_channels (uint32)
    20      4   num_cells (uint32)
    24     64   iso8601 timestamp (padded)
    88    var   per-channel names (length-prefixed UTF-8)
    ...
    N      var   scans: float64[num_ai + num_cells] × num_scans
```

This format is trivially memory-mappable for analysis tools and carries
enough metadata to reproduce a run.

### 4.6 Real-time priority

The legacy `GUI_RunModelForm.cpp` sets

```cpp
SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
```

Do **not** replicate that on the UI thread in the new design. Instead:

* **DAQ thread**: `THREAD_PRIORITY_TIME_CRITICAL` on Windows;
  `SCHED_FIFO` with a moderate priority (e.g. 80 on a 1–99 scale) on Linux
  via `pthread_setschedparam`. On macOS (simulation only) leave at
  default.
* **Writer thread**: normal priority, plus `ioprio_set(IOPRIO_CLASS_BE)`
  on Linux to keep it off the UI's back.
* **UI thread**: normal priority.

Never elevate the *whole process* priority — it starves device drivers.

### 4.7 UI integration

`RunDialog` becomes a thin orchestrator:

1. On **Start**: build the DAQ/writer/ring trio, start both threads,
   start a `QTimer` at 30 Hz that calls `drainUiRing()`.
2. `drainUiRing()`: snapshot the UI ring buffer and push only the points
   newer than the plot's last-seen cursor into `TracePanel`. Because
   `TracePlot` already uses a `RingBuffer` internally, pushing 30 Hz-worth
   of scans (≈ 1500 points per tick at 50 kHz) is fine.
3. On **Stop** (or when the requested duration elapses): tell the DAQ
   thread to stop, join it, then join the writer thread, then close the
   output file, then transcode binary → CSV if requested.

The user-visible contract:

* The progress bar and "scans acquired" counter update from the DAQ
  thread's atomics — they always reflect reality, even if the plot is
  behind.
* An "UI overflows" counter is shown somewhere non-alarming; a nonzero
  value just means the plot missed a few refreshes, not that data was lost.

### 4.8 Testing the new pipeline

1. **Unit tests** (`apps/test_core.cpp` is a good place to grow):
   * Round-trip the SPSC queue under heavy producer load, assert no
     reordering and no lost items.
   * Simulate a 500 ms UI stall and assert (a) queue A did not drop
     anything, (b) queue B's size stays bounded, (c) on resume, the UI
     pulls the most recent window.
2. **Mock-DAQ integration** — `DAQMock` already pretends to be a real
   device. Drive it at 50 kHz for 60 s through the new thread model and
   verify binary output size equals `rate × duration × sizeof(double) ×
   channels`, byte for byte.
3. **Hardware smoke test** — on a Windows machine with an M-series or
   X-series NI card, loop AO0 → AI0 with a short BNC. Write a 100 Hz sine
   from the dynamic-clamp loop and verify the captured trace matches with
   <1 ms latency and <0.1 % RMS error.

---

## 5. Coding conventions (carry-overs)

* The core is UI-agnostic. **Do not** include any `Q*` headers under
  `core/`. The mock DAQ already demonstrates how to consume core classes
  (`DAQMock` holds a `TModelCell`).
* `__fastcall` is defined away on non-MSVC compilers by `CMakeLists.txt`.
  New code should still mark virtual overrides `__fastcall` where it
  matches the surrounding style, but prefer plain `virtual` for new
  interfaces.
* The RK4 step cap (`TNetwork::SetMaxRK4Timestep`) is the one knob users
  turn when they see numerical instability. Leave its default at
  **0.01 ms** and surface it in any new run dialog.
* Logging of kinetic parameters uses `TDataLogger` (a wrapper around
  `std::ostringstream`). If you add a new current type, implement
  `GetParamLogHeader()` and append values to `params` in `DoUpdate()` so
  the per-current log stays consistent.

---

## 6. Open questions / pick-up points

1. **Chunk size vs. latency.** At 50 kHz AI, NI-DAQmx's default "all
   available samples" readout typically returns 50–200 scans per call on
   Windows. For dynamic clamp the closed-loop latency is
   `chunk_size / sample_rate`. We should expose a "max samples per read"
   control in `RunDialog` and plumb it through `DAQInterface::configureAI`.
   *(Status: still open. `DaqThread::Config::max_scans_per_read` exists;
   RunDialog hardcodes 1024. Adding a spinbox is a 15-line change.)*
2. **Per-scan vs. per-chunk network update.** Today the code updates the
   network once per chunk with the most recent V_m, then writes one AO
   value that is held until the next chunk. That is accurate for short
   chunks but distorts the effective conductance at long chunks. The
   right fix is to expose an option: (a) current behavior, (b) iterate
   the network per-scan inside the DAQ thread. (b) is expensive but
   physically correct. *(Status: still open — flagged as a deliberate
   design choice.)*
3. **Windows vs. Linux real-time.** On Linux, `SCHED_FIFO` requires
   `CAP_SYS_NICE` or a `/etc/security/limits.d` rule. *(Status: resolved.
   `rt_priority::elevate_current_thread()` returns a `Report` with
   `PermissionDenied` on EPERM; RunDialog surfaces it in the stats
   block. No crash on failure.)*
4. **Crash-safe disk writes.** *(Status: resolved. ScanWriter patches
   `num_scans_written` into the header on clean close; NtrxReader
   recovers the count from file size when the header is still 0.
   Covered by `test_scan_writer::crash_recovery_by_file_size`.)*
5. **Biological cell injection during "Before/After" phases.** The current
   code zeroes the AO during non-active phases. We should make this
   configurable — some protocols need a steady holding current throughout.
   *(Status: still open.)*
6. **fdatasync on Linux.** The writer thread calls `fflush` but not
   `fdatasync`, so a sudden power-off loses whatever was in the OS
   page cache. Expose it as a `ScanWriter::Config` flag.
   *(Status: still open.)*
7. **Hardware smoke test.** No one has yet loop-tested the pipeline
   against a real NI card. Needs a Windows box and a BNC loopback.
   *(Status: still open.)*

---

## 7. Quick pointers for common tasks

* **Add a new current type.** Subclass `TCurrent`; override `DoUpdate`,
  `GetParamLogHeader`, `ClassKey`, `CopyParamsFrom`. Register it with
  `GetCurrentFactory().registerBuilder(KEY, ...)` in a static initializer
  (see the bottom of `RT_HHCurrent.cpp` for the pattern). Add a matching
  `ui/editors/*dialog.h` and wire it into `NetworkEditor`.
* **Add a new cell type.** Subclass `TCell`; override `CalcVm`, `SetVm`,
  `IsVoltageDependent`, `AcceptsCurrents`, `Initialize`, `ClassKey`.
  Register with `GetCellFactory()`.
* **Add a new DAQ backend.** Subclass `DAQInterface` in a new file under
  `core/daq/`. Guard with `#ifdef HAVE_XYZ` mirroring the NI-DAQmx
  pattern in `CMakeLists.txt`.
* **Run the existing mock DAQ at the command line.** `apps/test_core.cpp`
  builds `build/test_core`, which runs a scripted HH network without Qt —
  useful for profiling the hot path in isolation.


---

## 8. Rig profiles (portable networks across labs)

Added after the DAQ pipeline shipped. Separates **what the model is**
(network `.json`) from **how this rig is wired** (`.rigprofile.json`)
so the same network can run on different benches without rewiring.

### 8.1 Data model

Defined in `core/io/rig_profile.h` — header-only.

| Type | Role |
|---|---|
| `RoleBinding` | One cell's wiring: `role`, `ai_channel`, `ao_channel`, `ai_gain`, `ao_gain`, `pos_limit_nA`, `neg_limit_nA`. |
| `RigProfile` | `name`, `description`, `std::vector<RoleBinding>`. `find(role)` returns `std::optional<RoleBinding>`. `role_names()` preserves insertion order. |
| `namespace rig_profile_json` | `binding_to_json` / `binding_from_json` / `to_json` / `from_json` / `save` / `load`. Schema version is baked into the file. |
| `RigProfileManager` | Singleton (`instance()`). Holds a `std::shared_ptr<RigProfile>` representing the active profile. `resolve(role)` returns `std::optional<RoleBinding>` — null if no profile, null if role missing. |

`TBiologicalCell` has a `Role()` / `SetRole()` pair. Empty role ⇒ no
rig-profile lookup.

### 8.2 Load-time resolution rules

In `networkFromJson()` (see `core/io/network_json.h`), the order is:

1. Read `aiChannel` / `aoChannel` / `aiGain` / `aoGain` /
   `posCurrentLimit` / `negCurrentLimit` from the JSON into the cell.
2. If the JSON has a `role` field, store it on the cell and ask
   `RigProfileManager::instance().resolve(role)`.
3. If that returns a binding, **overwrite** the cell's fields from the
   binding. If it returns `nullopt`, leave the fields as-read.

This ordering means profile bindings win when present, stored values
win when the profile doesn't know the role, and pre-role-era files
still work unchanged.

### 8.3 Profile discovery and activation (UI side)

`ui/editors/rigprofiledialog.h` (header-only, per project convention):

* `RigProfileDialog::profilesDir()` — static helper. Returns
  `QStandardPaths::AppConfigLocation + "/rig_profiles"`; creates it on
  demand. Every `*.rigprofile.json` file in that directory is one
  profile.
* `RigProfileDialog::restoreActiveFromSettings()` — static. Reads
  `QSettings["activeProfile"]`, loads that profile from disk,
  installs it on `RigProfileManager::instance()`. Called once from
  the `MainWindow` constructor.
* The dialog body is a standard list/editor: left pane shows all
  discovered profiles, right pane shows name + description + the
  bindings `QTableWidget`. `Set as Active` commits the editor,
  saves to disk, installs on the manager, and writes
  `QSettings["activeProfile"]`.

### 8.4 Biological cell UI integration

`ui/editors/biologicalcelldialog.h` gains a `QComboBox roleCombo`
seeded with the active profile's `role_names()`. Selecting a role
just writes that string into the cell via `SetRole()`; the runtime
resolution happens at network-load time, not dialog-save time. This
keeps the UI state and the on-disk state consistent.

If no profile is active, the combo shows *"— none —"* with a
diagnostic label and the channel/gain fields are the ground truth
(same behavior as before the feature existed).

### 8.5 Tests

`apps/test_rig_profile.cpp` — 10 tests:

* `json_roundtrip`, `file_roundtrip`, `empty_profile_is_valid` — format
  correctness.
* `find_returns_nullopt_for_unknown_role`, `role_names_preserves_order`
  — container semantics.
* `manager_active_profile_round_trip` — singleton state.
* `load_handles_missing_and_invalid_files` — error paths.
* `role_override_wins_when_profile_active`,
  `fallback_uses_stored_values_when_no_profile`,
  `fallback_when_role_not_in_active_profile` — the three
  resolution paths in `networkFromJson`, end-to-end.

### 8.6 Known rough edges

* The Rig Profile Manager doesn't validate that a channel string
  actually belongs to a real device — you can type `Dev9/ai17` and
  it'll be saved. The runtime `DAQmxCreateAIVoltageChan` call will
  fail in that case; we should surface that earlier.
* There's no "import / export rig profile" menu — users who want to
  share rig profiles must copy files from the config directory by
  hand. A File → Export Rig Profile menu item would be a small,
  pleasant addition.
* When an active profile is edited to remove a role that network
  files are currently using, NETrex silently falls back to stored
  values on next load. A "roles referenced but not defined" lint
  pass would be nicer than silent fallback.
