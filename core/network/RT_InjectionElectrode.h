#ifndef RT_INJECTIONELECTRODE_H
#define RT_INJECTIONELECTRODE_H

#include "RT_Electrode.h"
#include <cmath>

#define TInjectionElectrode_KEY L"Square Pulse Iinj Electrode"

class TInjectionElectrode : public TElectrode
{
private:
    double FInitDelay;   // ms
    double FDelay;       // ms (interpulse interval)
    double FDuration;    // ms
    double FAmplitude;   // nA
    int    FNumRepeats;  // -1 = forever

protected:
    void WriteToStream(std::ostream &stream) const override {}
    void ReadFromStream(std::istream &stream) override {}

public:
    double InitDelay() const { return FInitDelay; }
    void SetInitDelay(double v) { FInitDelay = v; }
    double Delay() const { return FDelay; }
    void SetDelay(double v) { FDelay = v; }
    double Duration() const { return FDuration; }
    void SetDuration(double v) { FDuration = v; }
    double Amplitude() const { return FAmplitude; }
    void SetAmplitude(double v) { FAmplitude = v; }
    int NumRepeats() const { return FNumRepeats; }
    void SetNumRepeats(int v) { FNumRepeats = v; }

    bool __fastcall Initialize(bool Reset) override { return TElectrode::Initialize(Reset); }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TInjectionElectrode_KEY);
        return Key;
    }

    double __fastcall DoUpdate(double step) override {
        if (FNumRepeats == 0) return 0.0;
        double period = FDelay + FDuration;
        double et = ElapsedTime() - FInitDelay;
        if (et > 0.0) {
            double whichrepeat = floor(et / period);
            double elapsed = fmod(et, period);
            if ((whichrepeat < FNumRepeats) || (FNumRepeats == -1)) {
                if (elapsed >= FDelay && elapsed < period) return FAmplitude;
            }
        }
        return 0.0;
    }

    TInjectionElectrode() : TElectrode(),
        FInitDelay(0), FDelay(100), FDuration(50), FAmplitude(1.0), FNumRepeats(-1) {}

    TInjectionElectrode(TCell *owner, const std::wstring &name) : TElectrode(owner, name),
        FInitDelay(0), FDelay(100), FDuration(50), FAmplitude(1.0), FNumRepeats(-1) {}
};

#endif
