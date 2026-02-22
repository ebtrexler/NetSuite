#ifndef RT_PLAYBACKCELL_H
#define RT_PLAYBACKCELL_H

#include "RT_Cell.h"
#include "RT_PlaybackWaveform.h"
#include <memory>

#define TPlaybackCell_KEY L"Vm Playback Cell"

/// Cell that plays back pre-recorded Vm from file. Accepts no currents or electrodes.
class TPlaybackCell : public TCell
{
private:
    std::unique_ptr<TPlaybackWaveform> FWaveform;

protected:
    void WriteToStream(std::ostream &) const override {}
    void ReadFromStream(std::istream &) override {}

public:
    TPlaybackWaveform &Waveform() {
        if (!FWaveform) FWaveform = std::make_unique<TPlaybackWaveform>();
        return *FWaveform;
    }

    double __fastcall SetVm(double Vm) override { return TCell::SetVm(Vm); }

    double __fastcall CalcVm(double step) override {
        double val = Waveform().EvaluateAtNext(step);
        SetVm(val);
        return val;
    }

    bool __fastcall IsVoltageDependent() override { return false; }
    const bool __fastcall AcceptsCurrents() const override { return false; }

    double __fastcall DoUpdate(double) override { return 0.0; }

    bool __fastcall Initialize(bool Reset) override { return Waveform().Initialize(Reset); }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TPlaybackCell_KEY);
        return Key;
    }

    TPlaybackCell() : TCell() {}
    TPlaybackCell(const std::wstring &name) : TCell(name) {}
};

#endif
