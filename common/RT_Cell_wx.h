#ifndef RT_CELL_WX_H
#define RT_CELL_WX_H

#include "RT_Base_wx.h"
#include "RT_CurrentUser_wx.h"

class TCell : public TRTBase, public TCurrentUser
{
protected:
    double m_capacitance;    // pF
    double m_restingPotential; // mV
    double m_threshold;      // mV
    double m_resistance;     // MOhm
    TCurrentsArray m_currents;
    
public:
    TCell(const std::wstring& name) : TRTBase(name), 
        m_capacitance(10.0), m_restingPotential(-70.0), 
        m_threshold(-55.0), m_resistance(100.0) {}
    virtual ~TCell() {}
    
    double GetCapacitance() const { return m_capacitance; }
    void SetCapacitance(double c) { m_capacitance = c; }
    double GetRestingPotential() const { return m_restingPotential; }
    void SetRestingPotential(double v) { m_restingPotential = v; }
    double GetThreshold() const { return m_threshold; }
    void SetThreshold(double t) { m_threshold = t; }
    double GetResistance() const { return m_resistance; }
    void SetResistance(double r) { m_resistance = r; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TCell";
        return key; 
    }
    
    // TCurrentUser interface
    virtual const TCurrentsArray& GetCurrents() const override { return m_currents; }
    virtual void AddCurrent(TCurrentPtr current) override;
    virtual void RemoveCurrent(TCurrent* current) override;
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

class TModelCell : public TCell
{
public:
    TModelCell(const std::wstring& name) : TCell(name) {}
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TModelCell";
        return key; 
    }
};



#endif