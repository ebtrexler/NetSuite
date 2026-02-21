# NetSuite Refactoring Plan: Business Logic vs. UI Separation

## Current Architecture Analysis

### ✅ KEEP AS-IS (Pure Business Logic - No UI Dependencies)

#### Core Engine (RT_*.h/cpp)
- `RT_Base.h/cpp` - Base class for all network objects
- `RT_Network.h/cpp` - Network orchestration and simulation engine
- `RT_Cell.h/cpp` - Cell base class
- `RT_Current.h/cpp` - Current base class  
- `RT_CurrentUser.h/cpp` - Current container base
- `RT_Electrode.h/cpp` - Electrode base class
- `RT_Synapse.h/cpp` - Synapse base class
- `RT_HHCurrent.h/cpp` - Hodgkin-Huxley current implementation
- `RT_HHKineticsFactor.h/cpp` - HH kinetics
- `RT_HHLinearPiecewiseCurrent.h/cpp` - Piecewise HH current
- `RT_HHLinearPiecewiseKineticsFactor.h/cpp` - Piecewise kinetics
- `RT_Utilities.h` - Utility functions

**Status**: ✅ Already clean - no VCL dependencies

#### Scientific/Math Libraries
- `MonotCubicInterpolator.h/cpp` - Interpolation algorithms
- `RARN.h/cpp` - Rational approximation
- `datalogger.h` - Data logging (may need minor refactor)

**Status**: ✅ Mostly clean

#### Model Fitting (Fit_*.h/cpp)
- `Fit_HHCurrent.h/cpp` - HH current fitting
- `Fit_LevenbergMaquardt.h/cpp` - Optimization algorithm
- `Fit_Thread_iGen.h/cpp` - Threading for fitting

**Status**: ⚠️ Check for UI dependencies in threading

---

### 🔄 REFACTOR (Mixed Logic + UI - Extract Business Logic)

#### Data Acquisition Layer
**Current**: `NI_DAQmx_Devices.h/cpp`, `NI_DAQmx_DevDescForm.h/cpp`

**Refactor to**:
```
core/daq/
├── daq_interface.h          // Abstract DAQ interface
├── daq_nidaqmx.h/cpp        // NI DAQmx implementation (no UI)
└── daq_config.h             // Configuration structures
```

**Extract**: Device enumeration, channel config, data streaming (pure logic)  
**Leave for UI**: Device selection dialogs, configuration forms

---

#### Network State & Serialization
**Current**: Mixed in `RT_Network.h/cpp` with some GUI coupling

**Refactor to**:
```
core/network/
├── network_state.h/cpp      // Network state management
├── network_serializer.h/cpp // Save/load (Boost serialization)
└── network_validator.h/cpp  // Network validation logic
```

**Extract**: All serialization, validation, state management  
**Leave for UI**: File dialogs, progress indicators

---

### ❌ REWRITE (Pure UI - Replace with New Framework)

#### Main Application Forms (GUI_*.h/cpp)
- `GUI_NetworkForm.h/cpp` - **Main network editor**
  - TreeView of network components
  - Visual network designer
  - Component property panels
  
- `GUI_RunModelForm.h/cpp` - **Simulation runner**
  - Real-time plotting
  - DAQ control panel
  - Parameter monitoring

#### Component Editors (GUI_RT_Edit_*.h/cpp)
- `GUI_RT_Edit_HHCurrent.h/cpp` - HH current editor
- `GUI_RT_Edit_HH2Current.h/cpp` - HH2 current editor
- `GUI_RT_Edit_BiologicalCell.h/cpp` - Biological cell editor
- `GUI_RT_Edit_ModelCell.h/cpp` - Model cell editor
- `GUI_RT_Edit_PlaybackCell.h/cpp` - Playback cell editor
- `GUI_RT_Edit_InjElectrode.h/cpp` - Injection electrode editor
- `GUI_RT_Edit_PlaybackElectrode.h/cpp` - Playback electrode editor
- `GUI_RT_Edit_GJCurrent.h/cpp` - Gap junction current editor
- `GUI_RT_Edit_GJSynapse.h/cpp` - Gap junction synapse editor
- `GUI_RT_Edit_GenBiDirSynapse.h/cpp` - Bidirectional synapse editor
- `GUI_RT_Edit_VoltageClampPIDCurrent.h/cpp` - Voltage clamp editor
- `GUI_RT_Edit_PlaybackCurrent.h/cpp` - Playback current editor

#### Utility Forms
- `GUI_PeriodicityEditor.h/cpp` - Periodicity settings
- `GUI_CirclePerimeterEditor.h/cpp` - Circle layout designer
- `GUI_SquareLatticeEditor.h/cpp` - Grid layout designer
- `GUI_CopyCurrentsForm.h/cpp` - Copy currents utility
- `GUI_TextFileImportOptions.h/cpp` - Import dialog
- `GUI_PlaybackWaveform.h/cpp` - Waveform viewer
- `GUI_PlaybackWaveformForm.h/cpp` - Waveform editor
- `GUI_VoltageClampFitForm.h/cpp` - Fitting interface
- `ChooseTypeAndName.h/cpp` - Object creation dialog

---

## Proposed New Architecture

```
NetSuite/
├── core/                          # Pure C++ business logic (no UI)
│   ├── network/
│   │   ├── RT_Base.h/cpp
│   │   ├── RT_Network.h/cpp
│   │   ├── RT_Cell.h/cpp
│   │   ├── RT_Current.h/cpp
│   │   ├── RT_Synapse.h/cpp
│   │   ├── RT_Electrode.h/cpp
│   │   └── ... (all RT_*.h/cpp)
│   ├── models/
│   │   ├── hh_current.h/cpp
│   │   ├── hh_kinetics.h/cpp
│   │   └── ... (model implementations)
│   ├── math/
│   │   ├── interpolator.h/cpp
│   │   ├── rarn.h/cpp
│   │   └── optimizer.h/cpp
│   ├── daq/
│   │   ├── daq_interface.h        # Abstract interface
│   │   ├── daq_nidaqmx.h/cpp      # NI implementation
│   │   └── daq_simulator.h/cpp    # Mock for testing
│   ├── io/
│   │   ├── serializer.h/cpp       # Network save/load
│   │   ├── data_logger.h/cpp      # Data logging
│   │   └── file_formats.h/cpp     # ABF, SON support
│   └── fitting/
│       ├── fitter_interface.h
│       ├── lm_optimizer.h/cpp
│       └── hh_fitter.h/cpp
│
├── ui/                            # New UI framework (Qt/ImGui/Web)
│   ├── main_window/
│   │   ├── network_editor.h/cpp   # Main editor
│   │   └── simulation_runner.h/cpp
│   ├── editors/
│   │   ├── cell_editor.h/cpp
│   │   ├── current_editor.h/cpp
│   │   ├── synapse_editor.h/cpp
│   │   └── electrode_editor.h/cpp
│   ├── visualizers/
│   │   ├── network_graph.h/cpp    # Visual network layout
│   │   ├── realtime_plotter.h/cpp # Real-time data plots
│   │   └── waveform_viewer.h/cpp
│   └── dialogs/
│       ├── daq_config_dialog.h/cpp
│       ├── import_dialog.h/cpp
│       └── fitting_dialog.h/cpp
│
├── bridge/                        # UI ↔ Core communication layer
│   ├── network_controller.h/cpp   # Mediates UI ↔ Network
│   ├── simulation_controller.h/cpp
│   ├── daq_controller.h/cpp
│   └── events.h                   # Event system for updates
│
└── apps/                          # Application entry points
    ├── netclamp/
    │   └── main.cpp
    ├── netsim/
    │   └── main.cpp
    └── netfit/
        └── main.cpp
```

---

## Practical Approach (No Borland Compiler)

### Step 0: Preserve Old Code
```bash
# Tag current state
git tag -a "vcl-original" -m "Last VCL version before refactor"

# Create reference branch (never build, just reference)
git branch vcl-reference
git checkout main

# Move GUI files to archive
mkdir -p archive/vcl_gui
mv GUI_*.cpp GUI_*.h GUI_*.dfm archive/vcl_gui/
```

### Step 1: Extract Core (Week 1-2)
Build **only** the core library without any GUI:

```cmake
# CMakeLists.txt
project(NetSuiteCore)

add_library(netsuite_core STATIC
    # Pure business logic - no VCL
    RT_Base.cpp
    RT_Network.cpp
    RT_Cell.cpp
    RT_Current.cpp
    RT_Synapse.cpp
    RT_Electrode.cpp
    RT_HHCurrent.cpp
    RT_HHKineticsFactor.cpp
    # ... all RT_*.cpp files
    MonotCubicInterpolator.cpp
    datalogger.cpp
)

target_include_directories(netsuite_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(netsuite_core Boost::serialization)
```

**Test immediately** - if core compiles without VCL, you're good!

### Step 2: Reference Old GUI for Requirements
Don't compile old GUI - **read it** to understand:
- What parameters each form edits
- What validation rules exist  
- What the UI workflow is
- What plots/visualizations are needed

Example - reading `GUI_RT_Edit_HHCurrent.cpp`:
```cpp
// OLD CODE (reference only, don't compile):
void __fastcall THHCurrentForm::GmaxEditExit(TObject *Sender) {
    double val = StrToFloat(GmaxEdit->Text);
    if (val < 0) {
        ShowMessage("Gmax must be positive!");
        return;
    }
    current_->Gmax(val);
}
```

**Extract requirements**:
- ✓ Gmax parameter needs editor
- ✓ Must be positive (validation)
- ✓ Show error message if invalid

### Step 3: Build New UI Directly (Week 3-8)
Implement in Qt/ImGui based on requirements extracted from old code:

```cpp
// NEW CODE (Qt):
void HHCurrentEditor::onGmaxChanged(double value) {
    std::string error;
    if (!current_->SetGmax(value, error)) {
        QMessageBox::warning(this, "Invalid Value", 
                           QString::fromStdString(error));
    }
}
```

---

### Phase 1: Create Clean Interfaces (Week 1-2)

1. **Define abstract interfaces** in `core/` without UI dependencies:
   ```cpp
   // core/network/network_interface.h
   class INetwork {
   public:
       virtual void AddCell(const std::string& name, CellType type) = 0;
       virtual void RemoveCell(const std::string& name) = 0;
       virtual void Update(double timestep) = 0;
       virtual std::vector<std::string> GetCellNames() const = 0;
       // ... pure virtual methods
   };
   ```

2. **Extract DAQ interface**:
   ```cpp
   // core/daq/daq_interface.h
   class IDAQDevice {
   public:
       virtual bool Initialize() = 0;
       virtual std::vector<std::string> GetAIChannels() = 0;
       virtual std::vector<std::string> GetAOChannels() = 0;
       virtual bool StartAcquisition() = 0;
       virtual bool ReadData(double* buffer, size_t samples) = 0;
       virtual bool WriteData(const double* buffer, size_t samples) = 0;
   };
   ```

3. **Create data transfer objects** (DTOs) for UI ↔ Core:
   ```cpp
   // bridge/dto/network_dto.h
   struct CellDTO {
       std::string name;
       std::string type;
       double voltage;
       std::map<std::string, double> parameters;
   };
   
   struct NetworkStateDTO {
       std::vector<CellDTO> cells;
       std::vector<SynapseDTO> synapses;
       std::vector<ElectrodeDTO> electrodes;
   };
   ```

### Phase 2: Refactor Core (Week 3-4)

1. **Remove VCL dependencies** from RT_* files:
   - Replace `AnsiString` → `std::string`
   - Replace `TStringList` → `std::vector<std::string>`
   - Remove any `#include <vcl.h>` or VCL headers

2. **Extract business logic** from GUI_* files:
   - Move validation logic to core
   - Move calculation logic to core
   - Keep only UI event handlers in GUI files

3. **Create controller layer**:
   ```cpp
   // bridge/network_controller.h
   class NetworkController {
   private:
       std::unique_ptr<TNetwork> network_;
       
   public:
       NetworkStateDTO GetState() const;
       void CreateCell(const std::string& name, CellType type);
       void UpdateCellParameter(const std::string& cell, 
                               const std::string& param, 
                               double value);
       // ... methods that UI calls
   };
   ```

### Phase 3: Build New UI (Week 5-8)

**Option A: Qt (Recommended)**
```cpp
// ui/main_window/network_editor.h
class NetworkEditorWindow : public QMainWindow {
    Q_OBJECT
    
private:
    NetworkController* controller_;
    QTreeView* networkTree_;
    QGraphicsView* networkView_;
    
private slots:
    void onCreateCell();
    void onCellSelected(const QString& name);
    void onParameterChanged(const QString& param, double value);
};
```

**Option B: Dear ImGui**
```cpp
// ui/main_window/network_editor.cpp
void NetworkEditor::Render() {
    if (ImGui::Begin("Network Editor")) {
        RenderNetworkTree();
        RenderPropertyPanel();
        RenderNetworkGraph();
    }
    ImGui::End();
}
```

### Phase 4: Direct Migration (Week 9-12)

- **No parallel development** - old VCL code is reference only
- Archive old GUI_* files for reference (don't compile them)
- Build new UI directly against extracted core
- Use old code as specification for behavior

---

## Key Extraction Patterns

### Pattern 1: Parameter Validation
**Before** (in GUI_RT_Edit_HHCurrent.cpp):
```cpp
void __fastcall THHCurrentForm::GmaxEditExit(TObject *Sender) {
    double val = StrToFloat(GmaxEdit->Text);
    if (val < 0) {
        ShowMessage("Gmax must be positive!");
        return;
    }
    current_->Gmax(val);
}
```

**After**:
```cpp
// core/models/hh_current.h
class THHCurrent {
    bool SetGmax(double value, std::string& error) {
        if (value < 0) {
            error = "Gmax must be positive";
            return false;
        }
        gmax_ = value;
        return true;
    }
};

// ui/editors/hh_current_editor.cpp (Qt)
void HHCurrentEditor::onGmaxChanged(double value) {
    std::string error;
    if (!current_->SetGmax(value, error)) {
        QMessageBox::warning(this, "Invalid Value", 
                           QString::fromStdString(error));
    }
}
```

### Pattern 2: Real-time Updates
**Before** (mixed in GUI_RunModelForm):
```cpp
void __fastcall TRunDynamicClampForm::Timer1Timer(TObject *Sender) {
    network_->Update(timestep_);
    UpdatePlots();  // VCL plotting
    UpdateDAQ();    // DAQ logic
}
```

**After**:
```cpp
// bridge/simulation_controller.h
class SimulationController {
    void Step() {
        network_->Update(timestep_);
        NotifyObservers();  // Observer pattern
    }
};

// ui/main_window/simulation_runner.cpp
void SimulationRunner::onTimerTick() {
    controller_->Step();
    // UI updates via observer callbacks
}
```

---

## Dependencies to Remove/Replace

### VCL Components → Replacements

| VCL Component | Qt Equivalent | ImGui Equivalent |
|---------------|---------------|------------------|
| TForm | QMainWindow | ImGui::Begin() |
| TTreeView | QTreeView | ImGui::TreeNode() |
| TValueListEditor | QTableWidget | ImGui::InputDouble() |
| TChart (plotting) | QCustomPlot | ImPlot |
| TPanel | QWidget | ImGui::BeginChild() |
| TButton | QPushButton | ImGui::Button() |
| TEdit | QLineEdit | ImGui::InputText() |
| TFileOpenDialog | QFileDialog | ImGuiFileDialog |

### Third-party Libraries
- **Graphics32** → Replace with Qt Graphics or ImGui
- **GothamSci PLOTPanel** → Replace with QCustomPlot or ImPlot
- **VCL TeeChart** → Replace with QCustomPlot or ImPlot

---

## Testing Strategy

1. **Unit tests for core** (no UI dependencies):
   ```cpp
   TEST(NetworkTest, AddCell) {
       TNetwork network;
       network.AddCell("Cell1", CellType::Model);
       EXPECT_EQ(network.GetCellCount(), 1);
   }
   ```

2. **Integration tests** for controller layer
3. **UI tests** (framework-specific)
4. **Regression tests** comparing old vs new behavior

---

## Migration Checklist

### Core Library
- [ ] Remove all VCL includes from RT_*.h files
- [ ] Replace VCL strings with std::string
- [ ] Extract validation logic from GUI files
- [ ] Create abstract DAQ interface
- [ ] Implement NI DAQmx without UI dependencies
- [ ] Create DTO classes for data transfer
- [ ] Build controller layer
- [ ] Add unit tests for core

### New UI
- [ ] Choose framework (Qt recommended)
- [ ] Implement main window
- [ ] Implement network tree view
- [ ] Implement network graph visualization
- [ ] Implement component editors
- [ ] Implement real-time plotting
- [ ] Implement DAQ configuration
- [ ] Implement file I/O dialogs
- [ ] Implement fitting interface

### Integration
- [ ] Connect UI to controllers
- [ ] Implement observer pattern for updates
- [ ] Test feature parity with old UI
- [ ] Performance testing
- [ ] User acceptance testing

---

## Estimated Effort

| Phase | Duration | Complexity |
|-------|----------|------------|
| Interface design | 2 weeks | Medium |
| Core refactoring | 2 weeks | Medium |
| New UI (Qt) | 4 weeks | High |
| Integration & testing | 2 weeks | Medium |
| Polish & documentation | 2 weeks | Low |
| **Total** | **12 weeks** | |

---

## Risk Mitigation

1. **Archive old code** - Keep VCL files as reference documentation only
2. **Incremental migration** - Port one form at a time, test thoroughly
3. **Automated testing** - Ensure core behavior matches old implementation
4. **Document old behavior** - Screenshot/record old UI before starting
5. **Version control** - Tag last VCL-compatible commit before refactor

---

## Next Steps

1. **Review this plan** - Validate approach
2. **Choose UI framework** - Qt vs ImGui vs Web
3. **Set up build system** - CMake for cross-platform
4. **Create core library** - Extract RT_* files first
5. **Prototype one editor** - Prove the architecture works
