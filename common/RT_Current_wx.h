#ifndef RT_CURRENT_WX_H
#define RT_CURRENT_WX_H

#include "RT_Base_wx.h"

class TCurrentUser;

class TCurrent : public TRTBase
{
protected:
    TCurrentUser* m_owner;
    double m_conductance;    // nS
    double m_reversalPotential; // mV
    bool m_isPeriodic;
    double m_period;         // ms
    
public:
    TCurrent(const std::wstring& name, TCurrentUser* owner = nullptr) 
        : TRTBase(name), m_owner(owner), m_conductance(1.0), 
          m_reversalPotential(0.0), m_isPeriodic(false), m_period(1000.0) {}
    virtual ~TCurrent() {}
    
    TCurrentUser* Owner() const { return m_owner; }
    void SetOwner(TCurrentUser* owner) { m_owner = owner; }
    
    double GetConductance() const { return m_conductance; }
    void SetConductance(double g) { m_conductance = g; }
    double GetReversalPotential() const { return m_reversalPotential; }
    void SetReversalPotential(double v) { m_reversalPotential = v; }
    bool IsPeriodic() const { return m_isPeriodic; }
    void SetPeriodic(bool p) { m_isPeriodic = p; }
    double GetPeriod() const { return m_period; }
    void SetPeriod(double p) { m_period = p; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TCurrent";
        return key; 
    }
    
    virtual std::string SerializeParameters() const;
    virtual void DeserializeParameters(const std::string& params);
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

class THHCurrent : public TCurrent
{
private:
    double m_gNa, m_gK, m_gL;  // Conductances
    double m_ENa, m_EK, m_EL;  // Reversal potentials
    
public:
    THHCurrent(const std::wstring& name, TCurrentUser* owner = nullptr)
        : TCurrent(name, owner), m_gNa(120.0), m_gK(36.0), m_gL(0.3),
          m_ENa(50.0), m_EK(-77.0), m_EL(-54.4) {}
    
    double GetGNa() const { return m_gNa; }
    void SetGNa(double g) { m_gNa = g; }
    double GetGK() const { return m_gK; }
    void SetGK(double g) { m_gK = g; }
    double GetGL() const { return m_gL; }
    void SetGL(double g) { m_gL = g; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"THHCurrent";
        return key; 
    }
    
    virtual std::string SerializeParameters() const override;
    virtual void DeserializeParameters(const std::string& params) override;
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

class TGapJunctionCurrent : public TCurrent
{
public:
    TGapJunctionCurrent(const std::wstring& name, TCurrentUser* owner = nullptr)
        : TCurrent(name, owner) {}
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TGapJunctionCurrent";
        return key; 
    }
};

class TPlaybackCurrent : public TCurrent
{
private:
    std::wstring m_filename;
    
public:
    TPlaybackCurrent(const std::wstring& name, TCurrentUser* owner = nullptr)
        : TCurrent(name, owner) {}
    
    const std::wstring& GetFilename() const { return m_filename; }
    void SetFilename(const std::wstring& f) { m_filename = f; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TPlaybackCurrent";
        return key; 
    }
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

class TVoltageClampPIDCurrent : public TCurrent
{
private:
    double m_targetVoltage;  // mV
    double m_kP, m_kI, m_kD; // PID gains
    
public:
    TVoltageClampPIDCurrent(const std::wstring& name, TCurrentUser* owner = nullptr)
        : TCurrent(name, owner), m_targetVoltage(-70.0), m_kP(1.0), m_kI(0.1), m_kD(0.01) {}
    
    double GetTargetVoltage() const { return m_targetVoltage; }
    void SetTargetVoltage(double v) { m_targetVoltage = v; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TVoltageClampPIDCurrent";
        return key; 
    }
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

#endif