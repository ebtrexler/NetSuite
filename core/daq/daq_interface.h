#ifndef DAQ_INTERFACE_H
#define DAQ_INTERFACE_H

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

/// Information about a single DAQ device
struct DAQDeviceInfo {
    std::string name;
    std::vector<std::string> aiChannels;
    std::vector<std::string> aoChannels;
    double maxAIRate = 0;
    double maxAORate = 0;
};

/// Thrown on DAQ errors
class DAQException : public std::runtime_error {
public:
    DAQException(const std::string &msg) : std::runtime_error(msg) {}
};

/// Abstract DAQ interface — implemented by real NI-DAQmx or mock backend
class DAQInterface {
public:
    virtual ~DAQInterface() = default;

    // --- Device enumeration ---
    virtual std::vector<DAQDeviceInfo> enumerateDevices() = 0;

    // --- Task configuration ---
    /// Configure AI: channels (comma-separated), requested sample rate.
    /// Returns coerced sample rate.
    virtual double configureAI(const std::string &channels, double sampleRate) = 0;

    /// Configure AO: channels (comma-separated).
    virtual void configureAO(const std::string &channels) = 0;

    // --- Runtime ---
    virtual void start() = 0;
    virtual void stop() = 0;

    /// Read all available AI samples. Returns number of scans read.
    /// Data is interleaved by channel (GroupByScanNumber).
    /// Buffer must be pre-allocated by caller.
    virtual int32_t readAI(double *buffer, int32_t bufferSize) = 0;

    /// Write a single scan of AO data (one value per channel).
    virtual void writeAO(const double *data, int numChannels) = 0;

    /// Get the number of AI channels in the current task
    virtual int numAIChannels() const = 0;
};

#endif
