#ifndef RT_PLAYBACKWAVEFORM_H
#define RT_PLAYBACKWAVEFORM_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

/// Loads and stores pre-recorded waveforms from CSV/text files.
/// Supports linear interpolation, scaling, offset, repeat, and delay.
class TPlaybackWaveform
{
private:
    std::string FFileName;
    double FScaleFactor;
    double FOffset;
    double FPlaybackRate;     // ms per sample
    double FFileDuration;     // total duration in ms
    double FWaveformDuration; // possibly altered duration
    double FDelayBeforeRepeat;
    int    FRepeatNumber;     // -1 = forever, >=1 = fixed
    int    FSelectedChannel;
    bool   FTXTTimeColumn;    // first column is time?
    bool   FTXTHeaderRow;     // first row is header?

    std::vector<double> FTimeData;
    std::vector<double> FValueData;
    std::vector<std::string> FChannelNames;
    int FNumChans;

    double ElapsedTime;
    int whichrep;

    double lerp(double t) const {
        if (FTimeData.empty()) return 0;
        if (t <= FTimeData.front()) return FValueData.front() * FScaleFactor + FOffset;
        if (t >= FTimeData.back()) return FValueData.back() * FScaleFactor + FOffset;
        // binary search
        size_t lo = 0, hi = FTimeData.size() - 1;
        while (hi - lo > 1) { size_t mid = (lo + hi) / 2; if (FTimeData[mid] <= t) lo = mid; else hi = mid; }
        double frac = (t - FTimeData[lo]) / (FTimeData[hi] - FTimeData[lo]);
        return (FValueData[lo] + frac * (FValueData[hi] - FValueData[lo])) * FScaleFactor + FOffset;
    }

public:
    TPlaybackWaveform() : FScaleFactor(1), FOffset(0), FPlaybackRate(1),
        FFileDuration(0), FWaveformDuration(0), FDelayBeforeRepeat(0),
        FRepeatNumber(-1), FSelectedChannel(0), FTXTTimeColumn(true),
        FTXTHeaderRow(true), FNumChans(0), ElapsedTime(0), whichrep(0) {}

    // Accessors
    const std::string &FileName() const { return FFileName; }
    void SetFileName(const std::string &f) { FFileName = f; LoadFile(); }
    double ScaleFactor() const { return FScaleFactor; }
    void SetScaleFactor(double v) { FScaleFactor = v; }
    double Offset() const { return FOffset; }
    void SetOffset(double v) { FOffset = v; }
    double PlaybackRate() const { return FPlaybackRate; }
    void SetPlaybackRate(double v) { FPlaybackRate = v; RebuildTime(); }
    double FileDuration() const { return FFileDuration; }
    double WaveformDuration() const { return FWaveformDuration; }
    void SetWaveformDuration(double v) { FWaveformDuration = v; }
    double DelayBeforeRepeat() const { return FDelayBeforeRepeat; }
    void SetDelayBeforeRepeat(double v) { FDelayBeforeRepeat = v; }
    int RepeatNumber() const { return FRepeatNumber; }
    void SetRepeatNumber(int v) { FRepeatNumber = v; }
    int SelectedChannel() const { return FSelectedChannel; }
    void SetSelectedChannel(int v) { FSelectedChannel = v; LoadFile(); }
    bool TXTTimeColumn() const { return FTXTTimeColumn; }
    void SetTXTTimeColumn(bool v) { FTXTTimeColumn = v; }
    bool TXTHeaderRow() const { return FTXTHeaderRow; }
    void SetTXTHeaderRow(bool v) { FTXTHeaderRow = v; }
    int NumChans() const { return FNumChans; }
    const std::vector<std::string> &ChannelNames() const { return FChannelNames; }
    const std::vector<double> &TimeData() const { return FTimeData; }
    const std::vector<double> &ValueData() const { return FValueData; }

    bool LoadFile() {
        FTimeData.clear(); FValueData.clear(); FChannelNames.clear(); FNumChans = 0;
        if (FFileName.empty()) return false;
        std::ifstream f(FFileName);
        if (!f.is_open()) return false;

        std::string line;
        std::vector<std::vector<double>> columns;
        bool firstLine = true;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            // Replace commas and tabs with spaces for uniform parsing
            for (char &c : line) if (c == ',' || c == '\t') c = ' ';
            std::istringstream iss(line);
            if (firstLine && FTXTHeaderRow) {
                std::string tok;
                while (iss >> tok) FChannelNames.push_back(tok);
                FNumChans = FTXTTimeColumn ? (int)FChannelNames.size() - 1 : (int)FChannelNames.size();
                firstLine = false;
                continue;
            }
            firstLine = false;
            std::vector<double> row;
            double v;
            while (iss >> v) row.push_back(v);
            if (columns.empty()) columns.resize(row.size());
            for (size_t i = 0; i < row.size() && i < columns.size(); i++)
                columns[i].push_back(row[i]);
        }
        if (columns.empty()) return false;
        if (FNumChans == 0) FNumChans = FTXTTimeColumn ? (int)columns.size() - 1 : (int)columns.size();

        int dataCol = FTXTTimeColumn ? FSelectedChannel + 1 : FSelectedChannel;
        if (dataCol < 0 || dataCol >= (int)columns.size()) dataCol = FTXTTimeColumn ? 1 : 0;

        if (FTXTTimeColumn && !columns.empty()) {
            FTimeData = columns[0];
        } else {
            FTimeData.resize(columns[0].size());
            for (size_t i = 0; i < FTimeData.size(); i++) FTimeData[i] = i * FPlaybackRate;
        }
        FValueData = columns[dataCol];
        FFileDuration = FTimeData.empty() ? 0 : FTimeData.back();
        FWaveformDuration = FFileDuration;
        return true;
    }

    void RebuildTime() {
        if (!FTXTTimeColumn && !FValueData.empty()) {
            FTimeData.resize(FValueData.size());
            for (size_t i = 0; i < FTimeData.size(); i++) FTimeData[i] = i * FPlaybackRate;
            FFileDuration = FTimeData.empty() ? 0 : FTimeData.back();
            FWaveformDuration = FFileDuration;
        }
    }

    double EvaluateAtNext(double step) {
        ElapsedTime += step;
        if (ElapsedTime < FWaveformDuration) return lerp(ElapsedTime);
        if (ElapsedTime >= FWaveformDuration + FDelayBeforeRepeat) {
            if (FRepeatNumber == -1) { ElapsedTime = 0; return lerp(0); }
            if (FRepeatNumber > 1) {
                whichrep++;
                if (whichrep <= FRepeatNumber) { ElapsedTime = 0; return lerp(0); }
            }
        }
        return 0.0;
    }

    bool Initialize(bool Reset) {
        if (Reset) { ElapsedTime = 0; whichrep = 0; }
        return true;
    }
};

#endif
