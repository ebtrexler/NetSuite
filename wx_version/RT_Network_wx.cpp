#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include "RT_BiologicalCell_wx.h"
#include "RT_Synapse_wx.h"
#include "RT_Electrode_wx.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

static TNetwork* g_network = nullptr;

TNetwork* GetNet()
{
    if (!g_network) {
        g_network = new TNetwork(L"DefaultNetwork");
    }
    return g_network;
}

const std::wstring& TNetwork::ClassKey() const
{
    static std::wstring key = L"TNetwork";
    return key;
}

wxPanel* TNetwork::CreateEditPanel(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    sizer->Add(new wxStaticText(panel, wxID_ANY, "Network Properties"), 0, wxALL, 5);
    sizer->Add(new wxStaticText(panel, wxID_ANY, wxString::Format("Cells: %zu", FCells.size())), 0, wxALL, 5);
    sizer->Add(new wxStaticText(panel, wxID_ANY, wxString::Format("Synapses: %zu", FSynapses.size())), 0, wxALL, 5);
    sizer->Add(new wxStaticText(panel, wxID_ANY, wxString::Format("Electrodes: %zu", FElectrodes.size())), 0, wxALL, 5);
    
    panel->SetSizer(sizer);
    return panel;
}

TCell* TNetwork::AddCell(const std::wstring& type, const std::wstring& name, int x, int y)
{
    TCellPtr cell;
    if (type == L"TModelCell") {
        cell = std::make_shared<TModelCell>(name);
    } else if (type == L"TBiologicalCell") {
        cell = std::make_shared<TBiologicalCell>(name);
    } else {
        cell = std::make_shared<TModelCell>(name);
    }
    
    cell->SetX(x);
    cell->SetY(y);
    FCells[name] = cell;
    return cell.get();
}

TSynapse* TNetwork::AddSynapse(const std::wstring& type, const std::wstring& name, 
                              const std::wstring& pre, const std::wstring& post, int x, int y)
{
    auto synapse = std::make_shared<TSynapse>(name);
    synapse->SetX(x);
    synapse->SetY(y);
    FSynapses[name] = synapse;
    return synapse.get();
}

TElectrode* TNetwork::AddElectrode(const std::wstring& type, const std::wstring& name,
                                  const std::wstring& cell, int x, int y)
{
    auto electrode = std::make_shared<TElectrode>(name);
    electrode->SetX(x);
    electrode->SetY(y);
    FElectrodes[name] = electrode;
    return electrode.get();
}

void TNetwork::RemoveCell(const std::wstring& name)
{
    FCells.erase(name);
}

void TNetwork::RemoveSynapse(const std::wstring& name)
{
    FSynapses.erase(name);
}

void TNetwork::RemoveElectrode(const std::wstring& name)
{
    FElectrodes.erase(name);
}

void TNetwork::Clear()
{
    FCells.clear();
    FSynapses.clear();
    FElectrodes.clear();
}