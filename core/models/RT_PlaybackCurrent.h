#ifndef RT_PLAYBACKCURRENT_H
#define RT_PLAYBACKCURRENT_H

#include "RT_Current.h"
#include "RT_PlaybackWaveform.h"
#include "RT_Utilities.h"
#include <memory>

#define TPLAYBACKCURRENT_KEY L"Playback Current"

/// Current whose conductance waveform is played back from file
class TPlaybackCurrent : public TCurrent
{
private:
    double F_E, F_Gmax, F_Gnoise;
    long idum;
    std::unique_ptr<TPlaybackWaveform> FWaveform;

protected:
    void WriteToStream(std::ostream &) const override {}
    void ReadFromStream(std::istream &) override {}

public:
    double E() const { return F_E; }
    void E(double v) { F_E = v; }
    double Gmax() const { return F_Gmax; }
    void Gmax(double v) { F_Gmax = v; }
    double Gnoise() const { return F_Gnoise; }
    void Gnoise(double v) { F_Gnoise = v; }

    TPlaybackWaveform &Waveform() {
        if (!FWaveform) FWaveform = std::make_unique<TPlaybackWaveform>();
        return *FWaveform;
    }

    bool Initialize(bool Reset) override {
        idum = -(std::rand() % 10000 + 1);
        return Waveform().Initialize(Reset);
    }

    const std::wstring & ClassKey() const override {
        static std::wstring Key(TPLAYBACKCURRENT_KEY);
        return Key;
    }

    const void GetParamLogHeader(std::vector<std::string> &params) const override {
        params.clear();
    }

    double DoUpdate(double step, double Vkin, double Vdrv,
                    std::vector<double> &params) override
    {
        params.clear();
        double gain = Waveform().EvaluateAtNext(step);
        double G = (F_Gnoise == 0.0) ? F_Gmax * gain
            : (F_Gmax + 0.01 * F_Gnoise * F_Gmax * gasdev(&idum)) * gain;
        return G * (Vdrv - F_E);
    }

    void CopyParamsFrom(const TCurrent * const) override {}

    TPlaybackCurrent() : TCurrent(nullptr, L""), F_E(0), F_Gmax(0), F_Gnoise(0), idum(-1) {}
    TPlaybackCurrent(TCurrentUser *owner, const std::wstring &name)
        : TCurrent(owner, name), F_E(0), F_Gmax(0), F_Gnoise(0), idum(-1) {}
};

#endif
