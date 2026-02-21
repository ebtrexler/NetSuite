# NetSuite Qt UI

Modern Qt-based user interface for NetSuite neural network modeling software.

## Features

- **Network Visualization**: Visual representation of neural network topology
- **Parameter Editors**: Dialog-based editors for all network components
- **File Management**: Save and load network configurations
- **Cross-Platform**: Runs on macOS, Windows, and Linux

## Current Functionality

### Implemented
- ✅ Main window with menu system
- ✅ Network visualization widget
- ✅ HH Current parameter editor
- ✅ Integration with core library
- ✅ Status bar with messages

### In Progress
- 🔄 Additional parameter editors (Cell, Synapse, Electrode)
- 🔄 Simulation controls
- 🔄 Data plotting

## Building

```bash
cd NetSuite/build
cmake .. -DBUILD_QT_UI=ON
make netsuite_qt
```

## Running

```bash
./qt_ui/netsuite_qt
```

## Usage

1. **Create Network**: File → New Network
2. **Edit Parameters**: Opens HH Current editor dialog
3. **View Network**: Central widget shows network topology
4. **Save/Load**: File → Save/Open (coming soon)

## Architecture

The Qt UI is completely separate from the core library:

```
Qt UI (netsuite_qt)
    ↓ uses
Core Library (libnetsuite_core.a)
    ↓ contains
Business Logic (RT_Network, RT_Cell, etc.)
```

This separation means:
- Core library can be used without Qt
- UI can be replaced or extended
- Testing is independent
- Cross-platform compatibility

## Adding New Dialogs

Follow the pattern in `hhcurrentdialog.h/cpp`:

```cpp
class MyDialog : public QDialog {
    Q_OBJECT
public:
    MyDialog(MyCoreObject *obj, QWidget *parent);
private slots:
    void accept() override;  // Validate and save
private:
    MyCoreObject *m_object;
    QLineEdit *paramEdit;
};
```

## Dependencies

- Qt5 (Widgets module)
- Core NetSuite library
- C++17 compiler

## License

Same as NetSuite core - GPL v3
