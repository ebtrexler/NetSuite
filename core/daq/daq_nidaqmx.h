#ifndef DAQ_NIDAQMX_H
#define DAQ_NIDAQMX_H

#ifdef HAVE_NIDAQMX

#include "daq_interface.h"
#include "nidaqmx.h"
#include <sstream>
#include <cstring>

/// Helper: check NI-DAQmx return code, throw on error
inline void daqmxCheck(int32 err) {
    if (err < 0) {
        char buf[2048] = {};
        DAQmxGetExtendedErrorInfo(buf, sizeof(buf));
        throw DAQException(std::string("NI-DAQmx error: ") + buf);
    }
}

class DAQNIDAQmx : public DAQInterface {
public:
    ~DAQNIDAQmx() override { stop(); }

    std::vector<DAQDeviceInfo> enumerateDevices() override {
        std::vector<DAQDeviceInfo> devices;
        char devNames[4096] = {};
        daqmxCheck(DAQmxGetSysDevNames(devNames, sizeof(devNames)));

        // Parse comma-separated device names
        std::istringstream ss(devNames);
        std::string name;
        while (std::getline(ss, name, ',')) {
            // Trim whitespace
            size_t s = name.find_first_not_of(" \t");
            if (s == std::string::npos) continue;
            name = name.substr(s, name.find_last_not_of(" \t") - s + 1);

            DAQDeviceInfo info;
            info.name = name;

            char buf[4096] = {};
            daqmxCheck(DAQmxGetDevAIPhysicalChans(name.c_str(), buf, sizeof(buf)));
            parseChannelList(buf, info.aiChannels);

            memset(buf, 0, sizeof(buf));
            daqmxCheck(DAQmxGetDevAOPhysicalChans(name.c_str(), buf, sizeof(buf)));
            parseChannelList(buf, info.aoChannels);

            daqmxCheck(DAQmxGetDevAIMaxMultiChanRate(name.c_str(), &info.maxAIRate));

            bool32 aoClkSupported = 0;
            daqmxCheck(DAQmxGetDevAOSampClkSupported(name.c_str(), &aoClkSupported));
            if (aoClkSupported)
                daqmxCheck(DAQmxGetDevAOMaxRate(name.c_str(), &info.maxAORate));

            devices.push_back(std::move(info));
        }
        return devices;
    }

    double configureAI(const std::string &channels, double sampleRate) override {
        daqmxCheck(DAQmxCreateTask("AI", &m_aiTask));
        daqmxCheck(DAQmxCreateAIVoltageChan(m_aiTask, channels.c_str(), "",
                   DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
        daqmxCheck(DAQmxCfgSampClkTiming(m_aiTask, "", sampleRate,
                   DAQmx_Val_Rising, DAQmx_Val_ContSamps,
                   static_cast<uInt64>(sampleRate * 10)));
        daqmxCheck(DAQmxSetReadReadAllAvailSamp(m_aiTask, true));

        // Get coerced rate
        float64 coerced = 0;
        daqmxCheck(DAQmxGetSampClkRate(m_aiTask, &coerced));
        m_sampleRate = coerced;

        uInt32 nCh = 0;
        daqmxCheck(DAQmxGetReadNumChans(m_aiTask, &nCh));
        m_numAI = static_cast<int>(nCh);

        return coerced;
    }

    void configureAO(const std::string &channels) override {
        daqmxCheck(DAQmxCreateTask("AO", &m_aoTask));
        daqmxCheck(DAQmxCreateAOVoltageChan(m_aoTask, channels.c_str(), "",
                   -10.0, 10.0, DAQmx_Val_Volts, ""));

        float64 maxRate = 0;
        daqmxCheck(DAQmxGetSampClkMaxRate(m_aoTask, &maxRate));
        daqmxCheck(DAQmxCfgSampClkTiming(m_aoTask, "", maxRate,
                   DAQmx_Val_Rising, DAQmx_Val_HWTimedSinglePoint, 5000));
    }

    void start() override {
        if (m_aoTask) daqmxCheck(DAQmxStartTask(m_aoTask));
        if (m_aiTask) daqmxCheck(DAQmxStartTask(m_aiTask));
        m_running = true;
    }

    void stop() override {
        if (!m_running) return;
        m_running = false;
        if (m_aiTask) { DAQmxStopTask(m_aiTask); DAQmxClearTask(m_aiTask); m_aiTask = 0; }
        if (m_aoTask) { DAQmxStopTask(m_aoTask); DAQmxClearTask(m_aoTask); m_aoTask = 0; }
    }

    int32_t readAI(double *buffer, int32_t bufferSize) override {
        int32 read = 0;
        daqmxCheck(DAQmxReadAnalogF64(m_aiTask, -1, 10.0,
                   DAQmx_Val_GroupByScanNumber, buffer, bufferSize, &read, NULL));
        return read;
    }

    void writeAO(const double *data, int numChannels) override {
        int32 written = 0;
        daqmxCheck(DAQmxWriteAnalogF64(m_aoTask, 1, false, 10.0,
                   DAQmx_Val_GroupByScanNumber, const_cast<float64*>(data),
                   &written, NULL));
    }

    int numAIChannels() const override { return m_numAI; }

private:
    static void parseChannelList(const char *text, std::vector<std::string> &out) {
        std::istringstream ss(text);
        std::string ch;
        while (std::getline(ss, ch, ',')) {
            size_t s = ch.find_first_not_of(" \t");
            if (s == std::string::npos) continue;
            out.push_back(ch.substr(s, ch.find_last_not_of(" \t") - s + 1));
        }
    }

    TaskHandle m_aiTask = 0;
    TaskHandle m_aoTask = 0;
    double m_sampleRate = 0;
    int m_numAI = 0;
    bool m_running = false;
};

#endif // HAVE_NIDAQMX
#endif
