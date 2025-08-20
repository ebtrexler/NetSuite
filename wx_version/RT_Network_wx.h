#ifndef RT_NETWORK_WX_H
#define RT_NETWORK_WX_H

#include "RT_Base_wx.h"
#include <map>
#include <memory>

class TCell;
class TSynapse;
class TElectrode;

typedef std::shared_ptr<TCell> TCellPtr;
typedef std::shared_ptr<TSynapse> TSynapsePtr;
typedef std::shared_ptr<TElectrode> TElectrodePtr;

typedef std::map<std::wstring, TCellPtr> TCellsMap;
typedef std::map<std::wstring, TSynapsePtr> TSynapsesMap;
typedef std::map<std::wstring, TElectrodePtr> TElectrodesMap;

class TNetwork : public TRTBase
{
private:
    TCellsMap FCells;
    TSynapsesMap FSynapses;
    TElectrodesMap FElectrodes;

public:
    TNetwork(const std::wstring& name = L"Network") : TRTBase(name) {}
    
    const TCellsMap& GetCells() const { return FCells; }
    const TSynapsesMap& GetSynapses() const { return FSynapses; }
    const TElectrodesMap& GetElectrodes() const { return FElectrodes; }
    
    TCell* AddCell(const std::wstring& type, const std::wstring& name, int x = 0, int y = 0);
    TSynapse* AddSynapse(const std::wstring& type, const std::wstring& name, 
                        const std::wstring& pre, const std::wstring& post, int x = 0, int y = 0);
    TElectrode* AddElectrode(const std::wstring& type, const std::wstring& name,
                            const std::wstring& cell, int x = 0, int y = 0);
    
    void RemoveCell(const std::wstring& name);
    void RemoveSynapse(const std::wstring& name);
    void RemoveElectrode(const std::wstring& name);
    
    void Clear();
    
    virtual const std::wstring& ClassKey() const override;
    virtual wxPanel* CreateEditPanel(wxWindow* parent) override;
};

TNetwork* GetNet();

#endif