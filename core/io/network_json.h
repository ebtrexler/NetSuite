#ifndef NETWORK_JSON_H
#define NETWORK_JSON_H

#include "json.hpp"
#include "RT_Network.h"
#include "RT_ModelCell.h"
#include "RT_PlaybackCell.h"
#include "RT_HHCurrent.h"
#include "RT_HH2Current.h"
#include "RT_VoltageClampPIDCurrent.h"
#include "RT_PlaybackCurrent.h"
#include "RT_HHKineticsFactor.h"
#include "RT_InjectionElectrode.h"
#include "RT_PlaybackElectrode.h"
#include "RT_GapJunctionSynapse.h"
#include "RT_GenBiDirSynapse.h"
#include "RT_GJCurrent.h"
#ifdef DAQ
#include "RT_BiologicalCell.h"
#endif
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

inline json kineticsToJson(THHKineticsFactor &f) {
    return { {"V0", f.V0()}, {"k", f.k()}, {"t_lo", f.t_lo()}, {"t_hi", f.t_hi()}, {"infMin", f.infMin()} };
}

inline void loadKinetics(json &kj, THHKineticsFactor &kf) {
    if (kj.contains("V0")) kf.V0(kj["V0"].get<double>());
    if (kj.contains("k")) kf.k(kj["k"].get<double>());
    if (kj.contains("t_lo")) kf.t_lo(kj["t_lo"].get<double>());
    if (kj.contains("t_hi")) kf.t_hi(kj["t_hi"].get<double>());
    if (kj.contains("infMin")) kf.infMin(kj["infMin"].get<double>());
}

inline json waveformToJson(TPlaybackWaveform &wf) {
    json j;
    j["fileName"] = wf.FileName();
    j["scaleFactor"] = wf.ScaleFactor();
    j["offset"] = wf.Offset();
    j["playbackRate"] = wf.PlaybackRate();
    j["repeatNumber"] = wf.RepeatNumber();
    j["delayBeforeRepeat"] = wf.DelayBeforeRepeat();
    j["selectedChannel"] = wf.SelectedChannel();
    j["txtTimeColumn"] = wf.TXTTimeColumn();
    j["txtHeaderRow"] = wf.TXTHeaderRow();
    return j;
}

inline void loadWaveform(json &j, TPlaybackWaveform &wf) {
    if (j.contains("txtTimeColumn")) wf.SetTXTTimeColumn(j["txtTimeColumn"].get<bool>());
    if (j.contains("txtHeaderRow")) wf.SetTXTHeaderRow(j["txtHeaderRow"].get<bool>());
    if (j.contains("selectedChannel")) wf.SetSelectedChannel(j["selectedChannel"].get<int>());
    if (j.contains("fileName")) wf.SetFileName(j["fileName"].get<std::string>());
    if (j.contains("scaleFactor")) wf.SetScaleFactor(j["scaleFactor"].get<double>());
    if (j.contains("offset")) wf.SetOffset(j["offset"].get<double>());
    if (j.contains("playbackRate")) wf.SetPlaybackRate(j["playbackRate"].get<double>());
    if (j.contains("repeatNumber")) wf.SetRepeatNumber(j["repeatNumber"].get<int>());
    if (j.contains("delayBeforeRepeat")) wf.SetDelayBeforeRepeat(j["delayBeforeRepeat"].get<double>());
}

inline json currentToJson(TCurrent *c) {
    json j;
    j["name"] = toUtf8(c->Name());
    j["classKey"] = toUtf8(c->ClassKey());
    j["active"] = c->IsActive();
    
    if (auto *hh = dynamic_cast<THHCurrent*>(c)) {
        j["Gmax"] = hh->Gmax();
        j["E"] = hh->E();
        j["Gnoise"] = hh->Gnoise();
        j["p"] = hh->p();
        j["q"] = hh->q();
        j["r"] = hh->r();
        j["m_kinetics"] = kineticsToJson(hh->get_m());
        j["h_kinetics"] = kineticsToJson(hh->get_h());
        j["n_kinetics"] = kineticsToJson(hh->get_n());
    }
    else if (auto *hh2 = dynamic_cast<THH2Current*>(c)) {
        j["Gmax"] = hh2->Gmax(); j["E"] = hh2->E(); j["Gnoise"] = hh2->Gnoise();
        j["p_pre"] = hh2->p_pre(); j["q_pre"] = hh2->q_pre(); j["r_pre"] = hh2->r_pre();
        j["p_post"] = hh2->p_post(); j["q_post"] = hh2->q_post(); j["r_post"] = hh2->r_post();
        j["Add_Dont_Multiply"] = hh2->Add_Dont_Multiply();
        j["Gmax2"] = hh2->Gmax2(); j["E2"] = hh2->E2(); j["Gnoise2"] = hh2->Gnoise2();
        j["m_pre_kinetics"] = kineticsToJson(hh2->m_pre);
        j["h_pre_kinetics"] = kineticsToJson(hh2->h_pre);
        j["n_pre_kinetics"] = kineticsToJson(hh2->n_pre);
        j["m_post_kinetics"] = kineticsToJson(hh2->m_post);
        j["h_post_kinetics"] = kineticsToJson(hh2->h_post);
        j["n_post_kinetics"] = kineticsToJson(hh2->n_post);
    }
    else if (auto *pid = dynamic_cast<TVoltageClampPIDCurrent*>(c)) {
        j["VCommand"] = pid->VCommand(); j["pGain"] = pid->pGain();
        j["iGain"] = pid->iGain(); j["dGain"] = pid->dGain();
        j["tau"] = pid->tau(); j["iMax"] = pid->iMax(); j["iMin"] = pid->iMin();
    }
    else if (auto *pb = dynamic_cast<TPlaybackCurrent*>(c)) {
        j["Gmax"] = pb->Gmax(); j["E"] = pb->E(); j["Gnoise"] = pb->Gnoise();
        j["waveform"] = waveformToJson(pb->Waveform());
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
    
    if (auto *mc = dynamic_cast<TModelCell*>(cell)) {
        j["capacitance"] = mc->Capacitance();
        j["initialVm"] = mc->InitialVm();
    }
    else if (auto *pc = dynamic_cast<TPlaybackCell*>(cell)) {
        j["waveform"] = waveformToJson(pc->Waveform());
    }
#ifdef DAQ
    else if (auto *bc = dynamic_cast<TBiologicalCell*>(cell)) {
        j["posCurrentLimit"] = bc->PosCurrentLimit();
        j["negCurrentLimit"] = bc->NegCurrentLimit();
    }
#endif
    
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
        if (auto *inj = dynamic_cast<TInjectionElectrode*>(e)) {
            ej["initDelay"] = inj->InitDelay();
            ej["delay"] = inj->Delay();
            ej["duration"] = inj->Duration();
            ej["amplitude"] = inj->Amplitude();
            ej["numRepeats"] = inj->NumRepeats();
        }
        else if (auto *pb = dynamic_cast<TPlaybackElectrode*>(e)) {
            ej["waveform"] = waveformToJson(pb->Waveform());
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
    try { GetCellFactory().registerBuilder(
        TPlaybackCell_KEY, TypeID<TPlaybackCell>(), TypeID<const std::wstring>()); } catch (...) {}
#ifdef DAQ
    try { GetCellFactory().registerBuilder(
        TBiologicalCell_KEY, TypeID<TBiologicalCell>(), TypeID<const std::wstring>()); } catch (...) {}
#endif
    try { GetCurrentFactory().registerBuilder(
        THHCurrent_KEY, TypeID<THHCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        THH2Current_KEY, TypeID<THH2Current>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        TVoltageClamp_PID_Current_KEY, TypeID<TVoltageClampPIDCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        TPLAYBACKCURRENT_KEY, TypeID<TPlaybackCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetElectrodeFactory().registerBuilder(
        TInjectionElectrode_KEY, TypeID<TInjectionElectrode>(),
        TypeID<TCell*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetElectrodeFactory().registerBuilder(
        TPlaybackElectrode_KEY, TypeID<TPlaybackElectrode>(),
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
        if (auto *pc = dynamic_cast<TPlaybackCell*>(cell)) {
            if (cj.contains("waveform")) loadWaveform(cj["waveform"], pc->Waveform());
        }
#ifdef DAQ
        if (auto *bc = dynamic_cast<TBiologicalCell*>(cell)) {
            if (cj.contains("posCurrentLimit")) bc->SetPosCurrentLimit(cj["posCurrentLimit"].get<double>());
            if (cj.contains("negCurrentLimit")) bc->SetNegCurrentLimit(cj["negCurrentLimit"].get<double>());
        }
#endif
        
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
                    if (curj.contains("m_kinetics")) loadKinetics(curj["m_kinetics"], hh->get_m());
                    if (curj.contains("h_kinetics")) loadKinetics(curj["h_kinetics"], hh->get_h());
                    if (curj.contains("n_kinetics")) loadKinetics(curj["n_kinetics"], hh->get_n());
                }
                if (auto *hh2 = dynamic_cast<THH2Current*>(cur)) {
                    if (curj.contains("Gmax")) hh2->Gmax(curj["Gmax"].get<double>());
                    if (curj.contains("E")) hh2->E(curj["E"].get<double>());
                    if (curj.contains("Gnoise")) hh2->Gnoise(curj["Gnoise"].get<double>());
                    if (curj.contains("p_pre")) hh2->p_pre(curj["p_pre"].get<double>());
                    if (curj.contains("q_pre")) hh2->q_pre(curj["q_pre"].get<double>());
                    if (curj.contains("r_pre")) hh2->r_pre(curj["r_pre"].get<double>());
                    if (curj.contains("p_post")) hh2->p_post(curj["p_post"].get<double>());
                    if (curj.contains("q_post")) hh2->q_post(curj["q_post"].get<double>());
                    if (curj.contains("r_post")) hh2->r_post(curj["r_post"].get<double>());
                    if (curj.contains("Add_Dont_Multiply")) hh2->Add_Dont_Multiply(curj["Add_Dont_Multiply"].get<int>());
                    if (curj.contains("Gmax2")) hh2->Gmax2(curj["Gmax2"].get<double>());
                    if (curj.contains("E2")) hh2->E2(curj["E2"].get<double>());
                    if (curj.contains("Gnoise2")) hh2->Gnoise2(curj["Gnoise2"].get<double>());
                    if (curj.contains("m_pre_kinetics")) loadKinetics(curj["m_pre_kinetics"], hh2->m_pre);
                    if (curj.contains("h_pre_kinetics")) loadKinetics(curj["h_pre_kinetics"], hh2->h_pre);
                    if (curj.contains("n_pre_kinetics")) loadKinetics(curj["n_pre_kinetics"], hh2->n_pre);
                    if (curj.contains("m_post_kinetics")) loadKinetics(curj["m_post_kinetics"], hh2->m_post);
                    if (curj.contains("h_post_kinetics")) loadKinetics(curj["h_post_kinetics"], hh2->h_post);
                    if (curj.contains("n_post_kinetics")) loadKinetics(curj["n_post_kinetics"], hh2->n_post);
                }
                if (auto *pid = dynamic_cast<TVoltageClampPIDCurrent*>(cur)) {
                    if (curj.contains("VCommand")) pid->VCommand(curj["VCommand"].get<double>());
                    if (curj.contains("pGain")) pid->pGain(curj["pGain"].get<double>());
                    if (curj.contains("iGain")) pid->iGain(curj["iGain"].get<double>());
                    if (curj.contains("dGain")) pid->dGain(curj["dGain"].get<double>());
                    if (curj.contains("tau")) pid->tau(curj["tau"].get<double>());
                    if (curj.contains("iMax")) pid->iMax(curj["iMax"].get<double>());
                    if (curj.contains("iMin")) pid->iMin(curj["iMin"].get<double>());
                }
                if (auto *pb = dynamic_cast<TPlaybackCurrent*>(cur)) {
                    if (curj.contains("Gmax")) pb->Gmax(curj["Gmax"].get<double>());
                    if (curj.contains("E")) pb->E(curj["E"].get<double>());
                    if (curj.contains("Gnoise")) pb->Gnoise(curj["Gnoise"].get<double>());
                    if (curj.contains("waveform")) loadWaveform(curj["waveform"], pb->Waveform());
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
                if (auto *pb = dynamic_cast<TPlaybackElectrode*>(el)) {
                    if (elj.contains("waveform")) loadWaveform(elj["waveform"], pb->Waveform());
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
