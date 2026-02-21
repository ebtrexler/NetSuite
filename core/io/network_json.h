#ifndef NETWORK_JSON_H
#define NETWORK_JSON_H

#include "json.hpp"
#include "RT_Network.h"
#include "RT_ModelCell.h"
#include "RT_HHCurrent.h"
#include "RT_HHKineticsFactor.h"
#include "RT_InjectionElectrode.h"
#include "RT_GapJunctionSynapse.h"
#include "RT_GenBiDirSynapse.h"
#include "RT_GJCurrent.h"
#include <fstream>
#include <codecvt>
#include <locale>

using json = nlohmann::json;

namespace NetworkJson {

inline std::string toUtf8(const std::wstring &ws) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

inline std::wstring toWide(const std::string &s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

inline json currentToJson(TCurrent *c) {
    json j;
    j["name"] = toUtf8(c->Name());
    j["classKey"] = toUtf8(c->ClassKey());
    j["active"] = c->IsActive();
    
    THHCurrent *hh = dynamic_cast<THHCurrent*>(c);
    if (hh) {
        j["Gmax"] = hh->Gmax();
        j["E"] = hh->E();
        j["Gnoise"] = hh->Gnoise();
        j["p"] = hh->p();
        j["q"] = hh->q();
        j["r"] = hh->r();
        
        auto &m = hh->get_m();
        j["m_kinetics"] = { {"V0", m.V0()}, {"k", m.k()}, {"t_lo", m.t_lo()}, {"t_hi", m.t_hi()}, {"infMin", m.infMin()} };
        auto &h = hh->get_h();
        j["h_kinetics"] = { {"V0", h.V0()}, {"k", h.k()}, {"t_lo", h.t_lo()}, {"t_hi", h.t_hi()}, {"infMin", h.infMin()} };
        auto &n = hh->get_n();
        j["n_kinetics"] = { {"V0", n.V0()}, {"k", n.k()}, {"t_lo", n.t_lo()}, {"t_hi", n.t_hi()}, {"infMin", n.infMin()} };
    }
    return j;
}

inline json cellToJson(TCell *cell) {
    json j;
    j["name"] = toUtf8(cell->Name());
    j["classKey"] = toUtf8(cell->ClassKey());
    j["active"] = cell->IsActive();
    j["x"] = cell->GetX();
    j["y"] = cell->GetY();
    
    TModelCell *mc = dynamic_cast<TModelCell*>(cell);
    if (mc) {
        j["capacitance"] = mc->Capacitance();
        j["initialVm"] = mc->InitialVm();
    }
    
    // Currents
    json currents = json::array();
    TCurrentsArray ca = cell->GetCurrents();
    for (auto *c : ca) {
        currents.push_back(currentToJson(c));
    }
    j["currents"] = currents;
    
    // Electrodes
    json electrodes = json::array();
    TElectrodesArray ea = cell->GetElectrodes();
    for (auto *e : ea) {
        json ej;
        ej["name"] = toUtf8(e->Name());
        ej["classKey"] = toUtf8(e->ClassKey());
        ej["active"] = e->IsActive();
        TInjectionElectrode *inj = dynamic_cast<TInjectionElectrode*>(e);
        if (inj) {
            ej["initDelay"] = inj->InitDelay();
            ej["delay"] = inj->Delay();
            ej["duration"] = inj->Duration();
            ej["amplitude"] = inj->Amplitude();
            ej["numRepeats"] = inj->NumRepeats();
        }
        electrodes.push_back(ej);
    }
    j["electrodes"] = electrodes;
    
    return j;
}

inline json networkToJson(TNetwork *net) {
    json j;
    j["name"] = toUtf8(net->Name());
    j["maxRK4Timestep"] = net->GetMaxRK4Timestep();
    
    // Cells
    json cells = json::array();
    const TCellsMap &cm = net->GetCells();
    for (auto it = cm.begin(); it != cm.end(); ++it) {
        cells.push_back(cellToJson(it->second.get()));
    }
    j["cells"] = cells;
    
    // Synapses
    json synapses = json::array();
    const TSynapsesMap &sm = net->GetSynapses();
    for (auto it = sm.begin(); it != sm.end(); ++it) {
        json sj;
        TSynapse *syn = it->second.get();
        sj["name"] = toUtf8(syn->Name());
        sj["classKey"] = toUtf8(syn->ClassKey());
        sj["pre"] = toUtf8(syn->Pre()->Name());
        sj["post"] = toUtf8(syn->Post()->Name());
        
        TGapJunctionSynapse *gj = dynamic_cast<TGapJunctionSynapse*>(syn);
        if (gj) {
            TCurrentsArray p2p = gj->PreToPostCurrents();
            TCurrentsArray p2r = gj->PostToPreCurrents();
            TGapJunctionCurrent *c1 = p2p.empty() ? nullptr : dynamic_cast<TGapJunctionCurrent*>(p2p[0]);
            TGapJunctionCurrent *c2 = p2r.empty() ? nullptr : dynamic_cast<TGapJunctionCurrent*>(p2r[0]);
            sj["gmax_pre2post"] = c1 ? c1->Gmax() : 0.0;
            sj["gmax_post2pre"] = c2 ? c2->Gmax() : 0.0;
        }
        synapses.push_back(sj);
    }
    if (!synapses.empty()) j["synapses"] = synapses;
    
    return j;
}

inline bool saveNetwork(TNetwork *net, const std::string &filename) {
    json j = networkToJson(net);
    std::ofstream f(filename);
    if (!f.is_open()) return false;
    f << j.dump(2);
    return true;
}

inline TNetwork* loadNetwork(const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return nullptr;
    
    json j = json::parse(f);
    
    TNetwork *net = new TNetwork(toWide(j["name"].get<std::string>()));
    
    if (j.contains("maxRK4Timestep")) {
        net->SetMaxRK4Timestep(j["maxRK4Timestep"].get<double>());
    }
    
    // Ensure factories are registered
    try { GetCellFactory().registerBuilder(
        TModelCell_KEY, TypeID<TModelCell>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        THHCurrent_KEY, TypeID<THHCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetElectrodeFactory().registerBuilder(
        TInjectionElectrode_KEY, TypeID<TInjectionElectrode>(),
        TypeID<TCell*const>(), TypeID<const std::wstring>()); } catch (...) {}
    
    for (auto &cj : j["cells"]) {
        std::wstring cellName = toWide(cj["name"].get<std::string>());
        std::wstring cellKey = toWide(cj["classKey"].get<std::string>());
        int x = cj.value("x", 0);
        int y = cj.value("y", 0);
        
        TCell *cell = net->AddCellToNet(cellKey, cellName, x, y);
        
        TModelCell *mc = dynamic_cast<TModelCell*>(cell);
        if (mc) {
            if (cj.contains("capacitance")) mc->SetCapacitance(cj["capacitance"].get<double>());
            if (cj.contains("initialVm")) mc->SetInitialVm(cj["initialVm"].get<double>());
        }
        
        // Load currents
        if (cj.contains("currents")) {
            for (auto &curj : cj["currents"]) {
                std::wstring curName = toWide(curj["name"].get<std::string>());
                std::wstring curKey = toWide(curj["classKey"].get<std::string>());
                
                TCurrent *cur = net->AddCurrentToCell(curKey, curName, cellName);
                
                THHCurrent *hh = dynamic_cast<THHCurrent*>(cur);
                if (hh) {
                    if (curj.contains("Gmax")) hh->Gmax(curj["Gmax"].get<double>());
                    if (curj.contains("E")) hh->E(curj["E"].get<double>());
                    if (curj.contains("Gnoise")) hh->Gnoise(curj["Gnoise"].get<double>());
                    if (curj.contains("p")) hh->p(curj["p"].get<double>());
                    if (curj.contains("q")) hh->q(curj["q"].get<double>());
                    if (curj.contains("r")) hh->r(curj["r"].get<double>());
                    
                    auto loadKinetics = [](json &kj, THHKineticsFactor &kf) {
                        if (kj.contains("V0")) kf.V0(kj["V0"].get<double>());
                        if (kj.contains("k")) kf.k(kj["k"].get<double>());
                        if (kj.contains("t_lo")) kf.t_lo(kj["t_lo"].get<double>());
                        if (kj.contains("t_hi")) kf.t_hi(kj["t_hi"].get<double>());
                        if (kj.contains("infMin")) kf.infMin(kj["infMin"].get<double>());
                    };
                    if (curj.contains("m_kinetics")) loadKinetics(curj["m_kinetics"], hh->get_m());
                    if (curj.contains("h_kinetics")) loadKinetics(curj["h_kinetics"], hh->get_h());
                    if (curj.contains("n_kinetics")) loadKinetics(curj["n_kinetics"], hh->get_n());
                }
            }
        }
        
        // Load electrodes
        if (cj.contains("electrodes")) {
            for (auto &elj : cj["electrodes"]) {
                std::wstring elName = toWide(elj["name"].get<std::string>());
                std::wstring elKey = toWide(elj["classKey"].get<std::string>());
                
                TElectrode *el = net->AddElectrodeToCell(elKey, elName, cellName);
                TInjectionElectrode *inj = dynamic_cast<TInjectionElectrode*>(el);
                if (inj) {
                    if (elj.contains("initDelay")) inj->SetInitDelay(elj["initDelay"].get<double>());
                    if (elj.contains("delay")) inj->SetDelay(elj["delay"].get<double>());
                    if (elj.contains("duration")) inj->SetDuration(elj["duration"].get<double>());
                    if (elj.contains("amplitude")) inj->SetAmplitude(elj["amplitude"].get<double>());
                    if (elj.contains("numRepeats")) inj->SetNumRepeats(elj["numRepeats"].get<int>());
                }
            }
        }
    }
    
    // Load synapses (after all cells exist)
    if (j.contains("synapses")) {
        try { GetCurrentFactory().registerBuilder(
            TGAPJUNCTIONCURRENT_KEY, TypeID<TGapJunctionCurrent>(),
            TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
        try { GetSynapseFactory().registerBuilder(
            TGAPJUNCTIONSYNAPSE_KEY, TypeID<TGapJunctionSynapse>(),
            TypeID<const std::wstring>(), TypeID<TCell*const>(), TypeID<TCell*const>()); } catch (...) {}
        try { GetSynapseFactory().registerBuilder(
            TGENBIDIRSYNAPSE_KEY, TypeID<TGenBiDirSynapse>(),
            TypeID<const std::wstring>(), TypeID<TCell*const>(), TypeID<TCell*const>()); } catch (...) {}
        
        for (auto &sj : j["synapses"]) {
            std::wstring synName = toWide(sj["name"].get<std::string>());
            std::wstring synKey = toWide(sj["classKey"].get<std::string>());
            std::wstring preName = toWide(sj["pre"].get<std::string>());
            std::wstring postName = toWide(sj["post"].get<std::string>());
            
            TSynapse *syn = net->AddSynapseBetweenCells(synKey, synName, preName, postName);
            
            TGapJunctionSynapse *gj = dynamic_cast<TGapJunctionSynapse*>(syn);
            if (gj) {
                TCurrentsArray p2p = gj->PreToPostCurrents();
                TCurrentsArray p2r = gj->PostToPreCurrents();
                TGapJunctionCurrent *c1 = p2p.empty() ? nullptr : dynamic_cast<TGapJunctionCurrent*>(p2p[0]);
                TGapJunctionCurrent *c2 = p2r.empty() ? nullptr : dynamic_cast<TGapJunctionCurrent*>(p2r[0]);
                if (c1 && sj.contains("gmax_pre2post")) c1->SetGmax(sj["gmax_pre2post"].get<double>());
                if (c2 && sj.contains("gmax_post2pre")) c2->SetGmax(sj["gmax_post2pre"].get<double>());
            }
        }
    }
    
    net->DescribeNetwork();
    return net;
}

} // namespace NetworkJson

#endif // NETWORK_JSON_H
