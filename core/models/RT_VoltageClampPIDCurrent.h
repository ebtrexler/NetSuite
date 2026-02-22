#ifndef RT_VOLTAGECLAMPPIDCURRENT_H
#define RT_VOLTAGECLAMPPIDCURRENT_H

#include "RT_Current.h"

#define TVoltageClamp_PID_Current_KEY L"Voltage Clamp (PID) Current"

/// PID-based voltage clamp current
class TVoltageClampPIDCurrent : public TCurrent
{
private:
    double FpGain, FiGain, FdGain;
    double FiMax, FiMin;
    double FiState, FdState;
    double Ftau;
    double FVCommand;

protected:
    void WriteToStream(std::ostream &) const override {}
    void ReadFromStream(std::istream &) override {}

public:
    double pGain() const { return FpGain; }
    void pGain(double v) { FpGain = v; }
    double iGain() const { return FiGain; }
    void iGain(double v) { FiGain = v; }
    double dGain() const { return FdGain; }
    void dGain(double v) { FdGain = v; }
    double iMax() const { return FiMax; }
    void iMax(double v) { FiMax = v; }
    double iMin() const { return FiMin; }
    void iMin(double v) { FiMin = v; }
    double tau() const { return Ftau; }
    void tau(double v) { Ftau = v; }
    double VCommand() const { return FVCommand; }
    void VCommand(double v) { FVCommand = v; }

    bool Initialize(bool Reset) override {
        if (Reset) { FiState = 0; FdState = 0; }
        return true;
    }

    const std::wstring & ClassKey() const override {
        static std::wstring Key(TVoltageClamp_PID_Current_KEY);
        return Key;
    }

    const void GetParamLogHeader(std::vector<std::string> &params) const override {
        params.clear();
    }

    double DoUpdate(double step, double Vkin, double Vdrv,
                    std::vector<double> &params) override
    {
        params.clear();
        double error = FVCommand - Vkin;
        // Proportional
        double pTerm = FpGain * error;
        // Integral with limiting
        FiState += error;
        if (FiState > FiMax) FiState = FiMax;
        else if (FiState < FiMin) FiState = FiMin;
        double iTerm = FiGain * FiState;
        // Derivative
        double dTerm = FdGain * (Vkin - FdState);
        FdState = Vkin;

        return (pTerm + iTerm + dTerm * step / Ftau) * -15.0;
    }

    void CopyParamsFrom(const TCurrent * const) override {}

    TVoltageClampPIDCurrent() : TCurrent(nullptr, L""),
        FpGain(0), FiGain(0), FdGain(0), FiMax(1), FiMin(-1),
        FiState(0), FdState(0), Ftau(1), FVCommand(0) {}

    TVoltageClampPIDCurrent(TCurrentUser *owner, const std::wstring &name) : TCurrent(owner, name),
        FpGain(0), FiGain(0), FdGain(0), FiMax(1), FiMin(-1),
        FiState(0), FdState(0), Ftau(1), FVCommand(0) {}
};

#endif
