# NETrex — Dynamic Clamp and Network Simulator

NETrex is a real-time neural network modeling and dynamic clamp application for neuroscience research. It combines the functionality of the legacy NetSim (simulation) and NetClamp (dynamic clamp) applications into a single unified program.

**Key features:**
- Build networks of model neurons with Hodgkin-Huxley conductances, synapses, gap junctions, and current injection electrodes
- Run pure simulations or real-time dynamic clamp experiments with NI-DAQmx hardware
- Real-time trace plotting, CSV data export, and configurable timing protocols
- Cross-platform: works on Windows, Linux, and macOS

**Author:** E. Brady Trexler, Ph.D.  
**License:** GNU General Public License v3  
**Website:** [hudsonvalleysci.com](https://hudsonvalleysci.com/)

---

## Download & Install

Pre-built binaries are available on the [Releases page](https://github.com/ebtrexler/NetSuite/releases/latest).

### Windows (recommended for dynamic clamp experiments)

1. Download `NETrex-Windows-x64.zip` from the latest release
2. Extract the zip to a folder (e.g., `C:\NETrex`)
3. Double-click `NETrex.exe` to run
4. **For dynamic clamp with NI hardware:** Install the free [NI-DAQmx Runtime](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html) from ni.com. NETrex will automatically detect your DAQ device.
5. **For simulation only:** No additional software needed — just run the app

### macOS

1. Download `NETrex-macOS.zip` from the latest release
2. Extract and open `NETrex.app`
3. If macOS blocks it: right-click → Open → click Open in the dialog
4. Note: macOS runs in simulation mode only (no NI-DAQmx hardware support). A mock DAQ backend is included for testing dynamic clamp networks.

### Linux

1. Download `NETrex-Linux-x64.zip` from the latest release
2. Extract and make executable: `chmod +x NETrex`
3. Run: `./NETrex`
4. Requires Qt 5 libraries (`sudo apt install libqt5widgets5` on Ubuntu/Debian)
5. **For dynamic clamp:** Install the [NI-DAQmx driver for Linux](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html)

---

## Quick Start (build from source)

```bash
git clone <repo-url> NETrex
cd NETrex
mkdir build && cd build
cmake ..
make NETrex
./NETrex
```

---

## Build Requirements

| Dependency | Required | Notes |
|---|---|---|
| C++17 compiler | Yes | GCC 7+, Clang 5+, MSVC 2017+ |
| CMake 3.15+ | Yes | Build system |
| Qt 5 (Widgets) | Yes | GUI framework |
| NI-DAQmx driver | No | Only needed for real hardware dynamic clamp |

All other dependencies (nlohmann/json, NI-DAQmx headers) are included in the repository.

---

## Platform-Specific Instructions

### Windows (with NI-DAQmx hardware)

This is the primary target for dynamic clamp experiments.

1. **Install prerequisites:**
   - [Visual Studio 2019 or 2022](https://visualstudio.microsoft.com/) (Community edition is free) — select "Desktop development with C++"
   - [CMake](https://cmake.org/download/) (3.15 or newer)
   - [Qt 5.15](https://www.qt.io/download-qt-installer) — install the MSVC 2019 64-bit component
   - [NI-DAQmx driver](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html) — install the full driver (not just Runtime)

2. **Set up environment:**
   ```cmd
   set CMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64
   ```
   Adjust the path to match your Qt installation.

3. **Build:**
   ```cmd
   git clone <repo-url> NETrex
   cd NETrex
   mkdir build && cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release --target NETrex
   ```

4. **Run:**
   ```cmd
   Release\NETrex.exe
   ```

   CMake will automatically detect the NI-DAQmx library and enable hardware support. You should see this during cmake configuration:
   ```
   -- NI-DAQmx found: C:/.../nidaqmx.lib
   ```

5. **Deploy (create standalone folder):**
   ```cmd
   mkdir deploy
   copy Release\NETrex.exe deploy\
   cd deploy
   windeployqt NETrex.exe
   ```
   The `deploy` folder can be zipped and shared — it contains all needed Qt DLLs.

### Linux (with NI-DAQmx hardware)

NI-DAQmx is supported on select Linux distributions (RHEL, CentOS, Ubuntu, openSUSE).

1. **Install prerequisites:**
   ```bash
   # Ubuntu/Debian
   sudo apt install build-essential cmake qt5-default

   # Fedora/RHEL
   sudo dnf install gcc-c++ cmake qt5-qtbase-devel
   ```

2. **Install NI-DAQmx (optional, for hardware):**
   Download the Linux driver from [ni.com](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html). Follow NI's installation instructions. The library installs to `/usr/lib/x86_64-linux-gnu/libnidaqmx.so`.

3. **Build:**
   ```bash
   git clone <repo-url> NETrex
   cd NETrex
   mkdir build && cd build
   cmake ..
   make NETrex
   ```

   If NI-DAQmx is installed, cmake will report:
   ```
   -- NI-DAQmx found: /usr/lib/x86_64-linux-gnu/libnidaqmx.so
   ```
   If not installed, it will use the mock backend for testing.

4. **Run:**
   ```bash
   ./NETrex
   ```

### macOS (development/simulation only)

NI-DAQmx is not available on macOS. NETrex runs with a mock DAQ backend that simulates a biological cell with Hodgkin-Huxley currents, allowing full development and testing of networks.

1. **Install prerequisites:**
   ```bash
   # Install Xcode command line tools
   xcode-select --install

   # Install Homebrew packages
   brew install cmake qt@5
   ```

2. **Build:**
   ```bash
   git clone <repo-url> NETrex
   cd NETrex
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@5)
   make NETrex
   ```

3. **Run:**
   ```bash
   ./NETrex
   ```

---

## How It Works

### Simulation Mode
When a network contains only model cells (no biological cells), NETrex runs as a pure simulator. The network is updated at the requested sample rate using RK4 integration, and results are plotted in real time.

### Dynamic Clamp Mode
When a network contains one or more biological cells, NETrex automatically switches to dynamic clamp mode:

1. Analog input channels read membrane voltages from the amplifier
2. The network model computes synaptic/intrinsic currents based on the measured voltages
3. Computed currents are written to analog output channels and injected into the cell

The timing protocol supports three phases:
- **Before:** DAQ is reading but no current is injected (baseline)
- **During:** Active dynamic clamp — currents are computed and injected
- **After:** DAQ is reading but current injection stops (recovery)

### Supported Cell Types
- **Model Cell** — Integrate-and-fire neuron with configurable currents
- **Vm Playback Cell** — Plays back a voltage waveform from a file
- **Biological Cell** — Represents a real neuron connected via DAQ hardware

### Supported Current/Synapse Types
- Hodgkin-Huxley conductance (HH, HH2)
- Gap junction
- Voltage clamp PID
- Generic bidirectional synapse
- Playback current waveform

---

## File Format

Networks are saved as JSON files (`.json`). Example:
```json
{
  "cells": [
    {
      "name": "Cell 1",
      "type": "Model Cell",
      "active": true,
      "currents": [...]
    }
  ],
  "electrodes": [...],
  "synapses": [...]
}
```

---

## Troubleshooting

**cmake can't find Qt5:**
Set `CMAKE_PREFIX_PATH` to your Qt installation:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt5
```

**NI-DAQmx not detected:**
Ensure the full NI-DAQmx driver is installed (not just Runtime). Check that `libnidaqmx` is in your library path. cmake will print a status message indicating whether the library was found.

**Mock backend on Windows/Linux:**
If you want to develop without hardware, NETrex works fine — it just uses the mock backend. No configuration needed.

---

## Building Redistributable Binaries

### Windows
```cmd
cmake --build . --config Release --target NETrex
mkdir deploy
copy Release\NETrex.exe deploy\
cd deploy
windeployqt NETrex.exe
```
Zip the `deploy` folder. Recipients need the NI-DAQmx Runtime (free download from ni.com) if using hardware.

### Linux
```bash
# Build with static Qt or use linuxdeployqt:
# https://github.com/probonopd/linuxdeployqt
make NETrex
linuxdeployqt NETrex -appimage
```
Produces a portable AppImage. Recipients need NI-DAQmx driver for hardware.

### macOS
```bash
make NETrex
macdeployqt NETrex.app -dmg
```
Produces a `.dmg` for distribution (simulation only, no DAQ hardware support).
