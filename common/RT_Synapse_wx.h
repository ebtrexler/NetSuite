#ifndef RT_SYNAPSE_WX_H
#define RT_SYNAPSE_WX_H

#include "RT_Base_wx.h"
#include "RT_CurrentUser_wx.h"

class TCell;

class TSynapse : public TRTBase, public TCurrentUser
{
private:
    TCell* FPre;
    TCell* FPost;
    double m_conductance;    // nS
    double m_reversalPotential; // mV
    double m_delay;          // ms
    double m_weight;         // unitless
    TCurrentsArray m_currents;
    
public:
    TSynapse(const std::wstring& name) : TRTBase(name), 
        FPre(nullptr), FPost(nullptr), m_conductance(1.0), 
        m_reversalPotential(0.0), m_delay(1.0), m_weight(1.0) {}
    virtual ~TSynapse() {}
    
    TCell* Pre() const { return FPre; }
    TCell* Post() const { return FPost; }
    void SetPre(TCell* cell) { FPre = cell; }
    void SetPost(TCell* cell) { FPost = cell; }
    
    double GetConductance() const { return m_conductance; }
    void SetConductance(double g) { m_conductance = g; }
    double GetReversalPotential() const { return m_reversalPotential; }
    void SetReversalPotential(double v) { m_reversalPotential = v; }
    double GetDelay() const { return m_delay; }
    void SetDelay(double d) { m_delay = d; }
    double GetWeight() const { return m_weight; }
    void SetWeight(double w) { m_weight = w; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TSynapse";
        return key; 
    }
    
    // TCurrentUser interface
    virtual const TCurrentsArray& GetCurrents() const override { return m_currents; }
    virtual void AddCurrent(TCurrentPtr current) override;
    virtual void RemoveCurrent(TCurrent* current) override;
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

#endif