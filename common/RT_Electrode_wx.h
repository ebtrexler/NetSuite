#ifndef RT_ELECTRODE_WX_H
#define RT_ELECTRODE_WX_H

#include "RT_Base_wx.h"

class TCell;

class TElectrode : public TRTBase
{
private:
    TCell* FOwner;
    double m_amplitude;      // nA
    double m_duration;       // ms
    double m_frequency;      // Hz
    bool m_isConstant;
    
public:
    TElectrode(const std::wstring& name) : TRTBase(name), 
        FOwner(nullptr), m_amplitude(0.1), m_duration(100.0), 
        m_frequency(10.0), m_isConstant(true) {}
    virtual ~TElectrode() {}
    
    TCell* Owner() const { return FOwner; }
    void SetOwner(TCell* cell) { FOwner = cell; }
    
    double GetAmplitude() const { return m_amplitude; }
    void SetAmplitude(double a) { m_amplitude = a; }
    double GetDuration() const { return m_duration; }
    void SetDuration(double d) { m_duration = d; }
    double GetFrequency() const { return m_frequency; }
    void SetFrequency(double f) { m_frequency = f; }
    bool IsConstant() const { return m_isConstant; }
    void SetConstant(bool c) { m_isConstant = c; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TElectrode";
        return key; 
    }
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

#endif