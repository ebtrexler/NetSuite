#ifndef RT_PLAYBACKELECTRODE_H
#define RT_PLAYBACKELECTRODE_H

#include "RT_Electrode.h"
#include "RT_PlaybackWaveform.h"
#include <memory>

#define TPlaybackElectrode_KEY L"Playback Electrode"

/// Electrode that plays back pre-recorded current waveform (nA) from file
class TPlaybackElectrode : public TElectrode
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

    double __fastcall DoUpdate(double step) override {
        return Waveform().EvaluateAtNext(step);
    }

    bool __fastcall Initialize(bool Reset) override { return Waveform().Initialize(Reset); }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TPlaybackElectrode_KEY);
        return Key;
    }

    TPlaybackElectrode() : TElectrode() {}
    TPlaybackElectrode(TCell *owner, const std::wstring &name) : TElectrode(owner, name) {}
};

#endif
