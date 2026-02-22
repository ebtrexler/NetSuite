#ifndef RT_BIOLOGICALCELL_H
#define RT_BIOLOGICALCELL_H

#include "RT_Cell.h"

#define TBiologicalCell_KEY L"Biological Cell"

/// DAQ-driven cell: Vm set externally via SetVm(), current output limited
class TBiologicalCell : public TCell
{
private:
    double FPosCurrentLimit;
    double FNegCurrentLimit;

protected:
    void WriteToStream(std::ostream &) const override {}
    void ReadFromStream(std::istream &) override {}

public:
    double PosCurrentLimit() const { return FPosCurrentLimit; }
    void SetPosCurrentLimit(double v) { FPosCurrentLimit = v; }
    double NegCurrentLimit() const { return FNegCurrentLimit; }
    void SetNegCurrentLimit(double v) { FNegCurrentLimit = v; }

    double __fastcall SetVm(double Vm) override { return TCell::SetVm(Vm); }
    double __fastcall CalcVm(double step) override { return TCell::CalcVm(step); }
    bool __fastcall IsVoltageDependent() override { return true; }
    const bool __fastcall AcceptsCurrents() const override { return true; }

    double __fastcall DoUpdate(double step) override {
        double I = TCell::DoUpdate(step);
        if (I > FPosCurrentLimit * FAOGain) I = FPosCurrentLimit * FAOGain;
        if (I < FNegCurrentLimit * FAOGain) I = FNegCurrentLimit * FAOGain;
        return I;
    }

    bool __fastcall Initialize(bool Reset) override { return true; }

    const std::wstring & __fastcall ClassKey() const override {
        static std::wstring Key(TBiologicalCell_KEY);
        return Key;
    }

    TBiologicalCell() : TCell(), FPosCurrentLimit(10), FNegCurrentLimit(-10) {}
    TBiologicalCell(const std::wstring &name) : TCell(name),
        FPosCurrentLimit(10), FNegCurrentLimit(-10) {}
};

#endif
