#ifndef RT_MODELCELL_H
#define RT_MODELCELL_H

#include "RT_Cell.h"

#define TModelCell_KEY L"Model Cell"

class TModelCell : public TCell
{
private:
    double FCapacitance;
    double FInitialVm;

    inline double f(double y) { return F_I / FCapacitance; }

    inline double rk4(double y_n, double step)
    {
        double k1, k2, k3, k4, t1, t2, t3;
        t1 = y_n + 0.5 * (k1 = step * f(y_n));
        t2 = y_n + 0.5 * (k2 = step * f(t1));
        t3 = y_n +       (k3 = step * f(t2));
        k4 =              step * f(t3);
        return y_n += (k1 + 2*k2 + 2*k3 + k4) / 6.0;
    }

public:
    TModelCell() : TCell(), FCapacitance(10), FInitialVm(-60) {}
    TModelCell(const std::wstring &name) : TCell(name), FCapacitance(10), FInitialVm(-60) {}

    double Capacitance() const { return FCapacitance; }
    void SetCapacitance(double c) { FCapacitance = c; }
    double InitialVm() const { return FInitialVm; }
    void SetInitialVm(double v) { FInitialVm = v; }

    double __fastcall CalcVm(double step) override
    {
        if (F_Vm > 200.0) F_Vm = 200.0;
        else if (F_Vm < -200.0) F_Vm = -200.0;
        else F_Vm = rk4(F_Vm, step);
        return F_Vm;
    }

    double __fastcall SetVm(double Vm) override { return 0; }
    bool __fastcall IsVoltageDependent() override { return false; }
    const bool __fastcall AcceptsCurrents() const override { return true; }

    bool __fastcall Initialize(bool Reset) override
    {
        if (Reset) { F_Vm = FInitialVm; F_I = 0; }
        return true;
    }

    const std::wstring & __fastcall ClassKey() const override
    {
        static std::wstring Key(TModelCell_KEY);
        return Key;
    }
};

#endif // RT_MODELCELL_H
