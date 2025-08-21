#ifndef RT_BIOLOGICALCELL_WX_H
#define RT_BIOLOGICALCELL_WX_H

#include "RT_Cell_wx.h"

class TBiologicalCell : public TCell
{
private:
    double m_posCurrentLimit;    // nA
    double m_negCurrentLimit;    // nA
    std::wstring m_aiChannel;    // AI channel name
    std::wstring m_aoChannel;    // AO channel name
    double m_aiGain;             // mV/V
    double m_aoGain;             // V/nA
    
public:
    TBiologicalCell(const std::wstring& name) : TCell(name), 
        m_posCurrentLimit(10.0), m_negCurrentLimit(-10.0),
        m_aiGain(1000.0), m_aoGain(1.0) {}
    virtual ~TBiologicalCell() {}
    
    // Current limits
    double GetPosCurrentLimit() const { return m_posCurrentLimit; }
    void SetPosCurrentLimit(double limit) { m_posCurrentLimit = limit; }
    double GetNegCurrentLimit() const { return m_negCurrentLimit; }
    void SetNegCurrentLimit(double limit) { m_negCurrentLimit = limit; }
    
    // DAQ channels and gains
    const std::wstring& GetAIChannel() const { return m_aiChannel; }
    void SetAIChannel(const std::wstring& channel) { m_aiChannel = channel; }
    const std::wstring& GetAOChannel() const { return m_aoChannel; }
    void SetAOChannel(const std::wstring& channel) { m_aoChannel = channel; }
    double GetAIGain() const { return m_aiGain; }
    void SetAIGain(double gain) { m_aiGain = gain; }
    double GetAOGain() const { return m_aoGain; }
    void SetAOGain(double gain) { m_aoGain = gain; }
    
    virtual const std::wstring& ClassKey() const override { 
        static std::wstring key = L"TBiologicalCell";
        return key; 
    }
    
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

#endif