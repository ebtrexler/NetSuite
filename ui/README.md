# NETrex Qt UI

Qt5-based graphical front-end for NETrex. This directory builds the
`NETrex` executable; the runtime engine lives in `core/` and is linked
in as `netrex_core`.

## Layout

```
ui/
├── main_window/         MainWindow: menu, toolbar, status bar, layout
├── visualizers/         NetworkView (graph), TracePanel/TracePlot (real-time)
└── editors/             Per-object parameter dialogs:
                           networkeditor.cpp        (tree hierarchy + dispatch)
                           modelcelldialog.h
                           biologicalcelldialog.h   (role dropdown, amp gains)
                           playbackcelldialog.h
                           hhcurrentdialog.h
                           hh2currentdialog.h
                           voltageclamppiddialog.h
                           playbackcurrentdialog.h
                           playbackwaveformdialog.h
                           electrodedialog.h
                           playbackelectrodedialog.h
                           synapsedialog.h          (gap junction conductances)
                           genbidirsynapsedialog.h  (chemical synapse currents)
                           rigprofiledialog.h       (rig wiring profiles)
                           rundialog.h              (dynamic-clamp run)
```

## Separation from core

The UI is completely separate from the core library. `core/` has no
Qt dependencies and can be used from command-line tools (`test_core`,
`test_spsc`, `test_scan_writer`, `test_daq_thread`, `test_rig_profile`,
`ntrx2csv`). The `NETrex` binary here is just one possible consumer.

## Adding a new dialog

Convention is header-only Qt dialogs (one file per dialog). Follow the
pattern of `hhcurrentdialog.h`:

```cpp
class MyDialog : public QDialog {
    Q_OBJECT
public:
    MyDialog(MyCoreObject *obj, QWidget *parent = nullptr);
private slots:
    void tryAccept();        // validate fields, write to obj, accept()
private:
    MyCoreObject *m_obj;
    // widget pointers ...
};
```

Then:

1. Add the header to the `add_executable(NETrex ...)` source list in
   `ui/CMakeLists.txt` so `AUTOMOC` generates the MOC code.
2. Include the header from wherever the dialog is opened (usually
   `networkeditor.cpp`).

## Build

From the repo root:

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@5)   # macOS
# or: cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/5.15.2/msvc2019_64  # Windows
cmake --build . --target NETrex
```

`BUILD_QT_UI=ON` is the default; pass `-DBUILD_QT_UI=OFF` to produce
only the core library and CLI test binaries.

## License

GPL v3, same as the rest of NETrex.
