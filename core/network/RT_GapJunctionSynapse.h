#ifndef RT_GAPJUNCTIONSYNAPSE_H
#define RT_GAPJUNCTIONSYNAPSE_H

#include "RT_Synapse.h"
#include "RT_GJCurrent.h"
#include "factory.h"

#define TGAPJUNCTIONSYNAPSE_KEY L"Gap Junction Synapse"

class TGapJunctionSynapse : public TSynapse
{
public:
    bool __fastcall Initialize(bool Reset) override { return true; }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TGAPJUNCTIONSYNAPSE_KEY);
        return Key;
    }

#ifndef NO_VCL
    void __fastcall PopulateEditForm() override {}
    bool __fastcall ValidateEditForm() override { return true; }
    void* const __fastcall GetEditForm() override { return nullptr; }
#endif

    __fastcall TGapJunctionSynapse() {}

    __fastcall TGapJunctionSynapse(const std::wstring &name, TCell * const pre, TCell * const post)
        : TSynapse(name, pre, post)
    {
        // Create one GJ current per direction
        try { GetCurrentFactory().registerBuilder(
            TGAPJUNCTIONCURRENT_KEY, TypeID<TGapJunctionCurrent>(),
            TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}

        std::wstring dir1 = pre->Name() + L" to " + post->Name();
        TCurrent *c1 = GetCurrentFactory().buildObj(
            TGAPJUNCTIONCURRENT_KEY, (TCurrentUser*)this, dir1);
        AddCurrent(c1, post);

        std::wstring dir2 = post->Name() + L" to " + pre->Name();
        TCurrent *c2 = GetCurrentFactory().buildObj(
            TGAPJUNCTIONCURRENT_KEY, (TCurrentUser*)this, dir2);
        AddCurrent(c2, pre);
    }

    virtual ~TGapJunctionSynapse() {}
};

#endif
