#ifndef RT_GENBIDIRSYNAPSE_H
#define RT_GENBIDIRSYNAPSE_H

#include "RT_Synapse.h"

#define TGENBIDIRSYNAPSE_KEY L"Generic Bi-Directional Synapse"

class TGenBiDirSynapse : public TSynapse
{
public:
    bool __fastcall Initialize(bool Reset) override { return true; }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TGENBIDIRSYNAPSE_KEY);
        return Key;
    }

#ifndef NO_VCL
    void __fastcall PopulateEditForm() override {}
    bool __fastcall ValidateEditForm() override { return true; }
    void* const __fastcall GetEditForm() override { return nullptr; }
#endif

    __fastcall TGenBiDirSynapse() {}

    __fastcall TGenBiDirSynapse(const std::wstring &name, TCell * const pre, TCell * const post)
        : TSynapse(name, pre, post) {}

    virtual ~TGenBiDirSynapse() {}
};

#endif
