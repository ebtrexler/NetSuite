// core/daq/daq_enumerate.h
//
// Backend-agnostic device enumeration. Returns the list of DAQ devices
// currently visible to the system — real NI-DAQmx devices when the
// driver is installed, the mock device list otherwise.
//
// This is cheap and side-effect-free: no tasks are created, no I/O is
// performed beyond the driver's existing device registry. It's safe to
// call from UI code on each dialog open.

#ifndef DAQ_ENUMERATE_H
#define DAQ_ENUMERATE_H

#include <vector>

#include "daq_interface.h"
#include "daq_mock.h"

#ifdef HAVE_NIDAQMX
#include "daq_nidaqmx.h"
#endif

/// Return every DAQ device the system can see right now.
///
/// Resolution order:
///   1. If NI-DAQmx is compiled in and the driver reports any devices,
///      return those.
///   2. Otherwise (Mac, Linux/Windows without the driver, or a driver
///      that reports zero devices), return the mock device.
///
/// The mock always yields at least one device named `MockDev1`, so the
/// returned vector is never empty.
inline std::vector<DAQDeviceInfo> enumerate_available_devices() {
#ifdef HAVE_NIDAQMX
    try {
        DAQNIDAQmx ni;
        auto devs = ni.enumerateDevices();
        if (!devs.empty()) return devs;
    } catch (...) {
        // Driver present at compile time but not responding at runtime
        // (e.g. device disconnected, simulated driver). Fall through.
    }
#endif
    DAQMock mock;
    return mock.enumerateDevices();
}

#endif  // DAQ_ENUMERATE_H
