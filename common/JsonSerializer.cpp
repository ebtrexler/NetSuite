#include "JsonSerializer.h"
#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include "RT_Synapse_wx.h"
#include "RT_Electrode_wx.h"
#include "RT_Current_wx.h"
#include "json.hpp"
using json = nlohmann::json;

bool JsonSerializer::SaveNetwork(const TNetwork* network, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << NetworkToJson(network);
    return file.good();
}

bool JsonSerializer::LoadNetwork(TNetwork* network, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return JsonToNetwork(json, network);
}

std::string JsonSerializer::NetworkToJson(const TNetwork* network)
{
    json j;
    j["name"] = std::string(network->Name().begin(), network->Name().end());
    
    // Cells
    j["cells"] = json::array();
    for (const auto& pair : network->GetCells()) {
        const TCell* cell = pair.second.get();
        json cellJson;
        cellJson["name"] = std::string(cell->Name().begin(), cell->Name().end());
        cellJson["type"] = std::string(cell->ClassKey().begin(), cell->ClassKey().end());
        cellJson["x"] = cell->GetX();
        cellJson["y"] = cell->GetY();
        cellJson["active"] = cell->IsActive();
        
        cellJson["currents"] = json::array();
        for (const auto& current : cell->GetCurrents()) {
            json currentJson;
            currentJson["name"] = std::string(current->Name().begin(), current->Name().end());
            currentJson["type"] = std::string(current->ClassKey().begin(), current->ClassKey().end());
            currentJson["params"] = json::parse(current->SerializeParameters());
            cellJson["currents"].push_back(currentJson);
        }
        j["cells"].push_back(cellJson);
    }
    
    // Synapses
    j["synapses"] = json::array();
    for (const auto& pair : network->GetSynapses()) {
        const TSynapse* synapse = pair.second.get();
        json synapseJson;
        synapseJson["name"] = std::string(synapse->Name().begin(), synapse->Name().end());
        synapseJson["type"] = std::string(synapse->ClassKey().begin(), synapse->ClassKey().end());
        synapseJson["pre"] = synapse->Pre() ? std::string(synapse->Pre()->Name().begin(), synapse->Pre()->Name().end()) : "";
        synapseJson["post"] = synapse->Post() ? std::string(synapse->Post()->Name().begin(), synapse->Post()->Name().end()) : "";
        synapseJson["active"] = synapse->IsActive();
        
        synapseJson["currents"] = json::array();
        for (const auto& current : synapse->GetCurrents()) {
            json currentJson;
            currentJson["name"] = std::string(current->Name().begin(), current->Name().end());
            currentJson["type"] = std::string(current->ClassKey().begin(), current->ClassKey().end());
            currentJson["params"] = json::parse(current->SerializeParameters());
            synapseJson["currents"].push_back(currentJson);
        }
        j["synapses"].push_back(synapseJson);
    }
    
    // Electrodes
    j["electrodes"] = json::array();
    for (const auto& pair : network->GetElectrodes()) {
        const TElectrode* electrode = pair.second.get();
        json electrodeJson;
        electrodeJson["name"] = std::string(electrode->Name().begin(), electrode->Name().end());
        electrodeJson["type"] = std::string(electrode->ClassKey().begin(), electrode->ClassKey().end());
        electrodeJson["owner"] = electrode->Owner() ? std::string(electrode->Owner()->Name().begin(), electrode->Owner()->Name().end()) : "";
        electrodeJson["active"] = electrode->IsActive();
        j["electrodes"].push_back(electrodeJson);
    }
    
    return j.dump(2);
}

bool JsonSerializer::JsonToNetwork(const std::string& jsonStr, TNetwork* network)
{
    try {
        network->Clear();
        json j = json::parse(jsonStr);
        
        // Load cells
        if (j.contains("cells")) {
            for (const auto& cellJson : j["cells"]) {
                std::string name = cellJson["name"];
                std::string type = cellJson["type"];
                int x = cellJson.value("x", 0);
                int y = cellJson.value("y", 0);
                
                TCell* cell = network->AddCell(std::wstring(type.begin(), type.end()), 
                                             std::wstring(name.begin(), name.end()), x, y);
                
                if (cellJson.contains("active")) {
                    cell->SetActive(cellJson["active"]);
                }
                
                // Load currents
                if (cellJson.contains("currents")) {
                    for (const auto& currentJson : cellJson["currents"]) {
                        std::string currentName = currentJson["name"];
                        std::string currentType = currentJson["type"];
                        
                        TCurrentPtr current;
                        if (currentType == "THHCurrent") {
                            current = std::make_shared<THHCurrent>(std::wstring(currentName.begin(), currentName.end()), cell);
                        } else if (currentType == "TGapJunctionCurrent") {
                            current = std::make_shared<TGapJunctionCurrent>(std::wstring(currentName.begin(), currentName.end()), cell);
                        } else if (currentType == "TPlaybackCurrent") {
                            current = std::make_shared<TPlaybackCurrent>(std::wstring(currentName.begin(), currentName.end()), cell);
                        } else if (currentType == "TVoltageClampPIDCurrent") {
                            current = std::make_shared<TVoltageClampPIDCurrent>(std::wstring(currentName.begin(), currentName.end()), cell);
                        } else {
                            current = std::make_shared<TCurrent>(std::wstring(currentName.begin(), currentName.end()), cell);
                        }
                        
                        if (currentJson.contains("params")) {
                            current->DeserializeParameters(currentJson["params"].dump());
                        }
                        
                        cell->AddCurrent(current);
                    }
                }
            }
        }
        
        // Load synapses
        if (j.contains("synapses")) {
            for (const auto& synapseJson : j["synapses"]) {
                std::string name = synapseJson["name"];
                std::string type = synapseJson["type"];
                std::string preName = synapseJson.value("pre", "");
                std::string postName = synapseJson.value("post", "");
                
                TSynapse* synapse = network->AddSynapse(std::wstring(type.begin(), type.end()), 
                                                       std::wstring(name.begin(), name.end()), L"", L"", 0, 0);
                
                if (synapseJson.contains("active")) {
                    synapse->SetActive(synapseJson["active"]);
                }
                
                // Set connections
                if (!preName.empty() && !postName.empty()) {
                    auto& cells = network->GetCells();
                    auto preIt = cells.find(std::wstring(preName.begin(), preName.end()));
                    auto postIt = cells.find(std::wstring(postName.begin(), postName.end()));
                    if (preIt != cells.end()) synapse->SetPre(preIt->second.get());
                    if (postIt != cells.end()) synapse->SetPost(postIt->second.get());
                }
                
                // Load currents
                if (synapseJson.contains("currents")) {
                    for (const auto& currentJson : synapseJson["currents"]) {
                        std::string currentName = currentJson["name"];
                        std::string currentType = currentJson["type"];
                        
                        TCurrentPtr current;
                        if (currentType == "THHCurrent") {
                            current = std::make_shared<THHCurrent>(std::wstring(currentName.begin(), currentName.end()), synapse);
                        } else if (currentType == "TGapJunctionCurrent") {
                            current = std::make_shared<TGapJunctionCurrent>(std::wstring(currentName.begin(), currentName.end()), synapse);
                        } else if (currentType == "TPlaybackCurrent") {
                            current = std::make_shared<TPlaybackCurrent>(std::wstring(currentName.begin(), currentName.end()), synapse);
                        } else if (currentType == "TVoltageClampPIDCurrent") {
                            current = std::make_shared<TVoltageClampPIDCurrent>(std::wstring(currentName.begin(), currentName.end()), synapse);
                        } else {
                            current = std::make_shared<TCurrent>(std::wstring(currentName.begin(), currentName.end()), synapse);
                        }
                        
                        if (currentJson.contains("params")) {
                            current->DeserializeParameters(currentJson["params"].dump());
                        }
                        
                        synapse->AddCurrent(current);
                    }
                }
            }
        }
        
        // Load electrodes
        if (j.contains("electrodes")) {
            for (const auto& electrodeJson : j["electrodes"]) {
                std::string name = electrodeJson["name"];
                std::string type = electrodeJson["type"];
                std::string ownerName = electrodeJson.value("owner", "");
                
                TElectrode* electrode = network->AddElectrode(std::wstring(type.begin(), type.end()), 
                                                             std::wstring(name.begin(), name.end()), L"", 0, 0);
                
                if (electrodeJson.contains("active")) {
                    electrode->SetActive(electrodeJson["active"]);
                }
                
                // Set owner
                if (!ownerName.empty()) {
                    auto& cells = network->GetCells();
                    auto ownerIt = cells.find(std::wstring(ownerName.begin(), ownerName.end()));
                    if (ownerIt != cells.end()) {
                        electrode->SetOwner(ownerIt->second.get());
                    }
                }
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}