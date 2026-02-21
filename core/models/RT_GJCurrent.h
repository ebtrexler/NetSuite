#ifndef RT_GJCURRENT_H
#define RT_GJCURRENT_H

#include "RT_Current.h"

#define TGAPJUNCTIONCURRENT_KEY L"Gap Junction Current"

class TGapJunctionCurrent : public TCurrent
{
private:
    double F_Gmax;

protected:
    void __fastcall WriteToStream(std::ostream &stream) const override {}
    void __fastcall ReadFromStream(std::istream &stream) override {}
    const void __fastcall GetParamLogHeader(std::vector<std::string>& params) const override { params.clear(); }

public:
    double Gmax() const { return F_Gmax; }
    void SetGmax(double g) { F_Gmax = g; }

    double __fastcall DoUpdate(double step, double Vpre, double Vpost,
                               std::vector<double> &params) override
    {
        params.clear();
        return F_Gmax * (Vpost - Vpre); // µS * mV = nA
    }

    bool __fastcall Initialize(bool Reset) override { return true; }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TGAPJUNCTIONCURRENT_KEY);
        return Key;
    }

#ifndef NO_VCL
    void __fastcall PopulateEditForm() override {}
    bool __fastcall ValidateEditForm() override { return true; }
    void* const __fastcall GetEditForm() override { return nullptr; }
#endif

    __fastcall TGapJunctionCurrent() : TCurrent(nullptr, L""), F_Gmax(0) {}
    __fastcall TGapJunctionCurrent(TCurrentUser *owner, const std::wstring &name)
        : TCurrent(owner, name), F_Gmax(0) {}

    void __fastcall CopyParamsFrom(const TCurrent * const source) override {
        if (this != source && ClassType() == source->ClassType())
            F_Gmax = ((TGapJunctionCurrent*)source)->F_Gmax;
    }

    virtual ~TGapJunctionCurrent() {}
};

#endif
