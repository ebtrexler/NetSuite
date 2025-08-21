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
            currentJson["params"] = current->SerializeParameters();
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
            currentJson["params"] = current->SerializeParameters();
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
    network->Clear();
    
    // Parse cells with positions
    size_t pos = 0;
    while ((pos = json.find("{\n      \"name\":", pos)) != std::string::npos) {
        size_t nameStart = json.find("\"", pos + 15) + 1;
        size_t nameEnd = json.find("\"", nameStart);
        std::string name = json.substr(nameStart, nameEnd - nameStart);
        
        size_t typeStart = json.find("\"type\": \"", nameEnd) + 9;
        size_t typeEnd = json.find("\"", typeStart);
        std::string type = json.substr(typeStart, typeEnd - typeStart);
        
        int x = 0, y = 0;
        size_t xPos = json.find("\"x\": ", typeEnd);
        if (xPos != std::string::npos && xPos < json.find("}", typeEnd)) {
            x = std::stoi(json.substr(xPos + 5, json.find(",", xPos) - xPos - 5));
        }
        
        size_t yPos = json.find("\"y\": ", xPos);
        if (yPos != std::string::npos && yPos < json.find("}", typeEnd)) {
            y = std::stoi(json.substr(yPos + 5, json.find(",", yPos) - yPos - 5));
        }
        
        if (type.find("Cell") != std::string::npos) {
            TCell* cell = network->AddCell(std::wstring(type.begin(), type.end()), 
                                         std::wstring(name.begin(), name.end()), x, y);
            
            // Load currents for this cell
            size_t currentsPos = json.find("\"currents\": [", nameEnd);
            if (currentsPos != std::string::npos && currentsPos < json.find("}", nameEnd)) {
                size_t currentPos = currentsPos;
                while ((currentPos = json.find("{\n          \"name\":", currentPos)) != std::string::npos) {
                    size_t currentNameStart = json.find("\"", currentPos + 18) + 1;
                    size_t currentNameEnd = json.find("\"", currentNameStart);
                    std::string currentName = json.substr(currentNameStart, currentNameEnd - currentNameStart);
                    
                    size_t currentTypeStart = json.find("\"type\": \"", currentNameEnd) + 9;
                    size_t currentTypeEnd = json.find("\"", currentTypeStart);
                    std::string currentType = json.substr(currentTypeStart, currentTypeEnd - currentTypeStart);
                    
                    // Create current based on type
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
                    
                    size_t paramsStart = json.find("\"params\": \"", currentTypeEnd) + 11;
                    size_t paramsEnd = json.find("\"", paramsStart);
                    if (paramsStart != std::string::npos && paramsEnd != std::string::npos) {
                        std::string params = json.substr(paramsStart, paramsEnd - paramsStart);
                        current->DeserializeParameters(params);
                    }
                    
                    cell->AddCurrent(current);
                    
                    currentPos = json.find("}", currentTypeEnd) + 1;
                    if (json.find("]", currentPos) < json.find("{\n          \"name\":", currentPos)) break;
                }
            }
        }
        
        pos = nameEnd;
    }
    
    // Parse synapses with connections
    pos = json.find("\"synapses\":");
    if (pos != std::string::npos) {
        while ((pos = json.find("{\n      \"name\":", pos)) != std::string::npos) {
            size_t nameStart = json.find("\"", pos + 15) + 1;
            size_t nameEnd = json.find("\"", nameStart);
            std::string name = json.substr(nameStart, nameEnd - nameStart);
            
            size_t typeStart = json.find("\"type\": \"", nameEnd) + 9;
            size_t typeEnd = json.find("\"", typeStart);
            std::string type = json.substr(typeStart, typeEnd - typeStart);
            
            std::string preName, postName;
            size_t prePos = json.find("\"pre\": \"", typeEnd);
            if (prePos != std::string::npos && prePos < json.find("}", typeEnd)) {
                size_t preStart = prePos + 8;
                size_t preEndPos = json.find("\"", preStart);
                preName = json.substr(preStart, preEndPos - preStart);
            }
            
            size_t postPos = json.find("\"post\": \"", prePos);
            if (postPos != std::string::npos && postPos < json.find("}", typeEnd)) {
                size_t postStart = postPos + 9;
                size_t postEndPos = json.find("\"", postStart);
                postName = json.substr(postStart, postEndPos - postStart);
            }
            
            TSynapse* synapse = network->AddSynapse(std::wstring(type.begin(), type.end()), 
                                                   std::wstring(name.begin(), name.end()), L"", L"", 0, 0);
            
            // Set connections
            if (!preName.empty() && !postName.empty()) {
                auto& cells = network->GetCells();
                auto preIt = cells.find(std::wstring(preName.begin(), preName.end()));
                auto postIt = cells.find(std::wstring(postName.begin(), postName.end()));
                if (preIt != cells.end()) synapse->SetPre(preIt->second.get());
                if (postIt != cells.end()) synapse->SetPost(postIt->second.get());
            }
            
            // Load currents for this synapse
            size_t currentsPos = json.find("\"currents\": [", typeEnd);
            if (currentsPos != std::string::npos && currentsPos < json.find("}", typeEnd)) {
                size_t currentPos = currentsPos;
                while ((currentPos = json.find("{\n          \"name\":", currentPos)) != std::string::npos) {
                    size_t currentNameStart = json.find("\"", currentPos + 18) + 1;
                    size_t currentNameEnd = json.find("\"", currentNameStart);
                    std::string currentName = json.substr(currentNameStart, currentNameEnd - currentNameStart);
                    
                    size_t currentTypeStart = json.find("\"type\": \"", currentNameEnd) + 9;
                    size_t currentTypeEnd = json.find("\"", currentTypeStart);
                    std::string currentType = json.substr(currentTypeStart, currentTypeEnd - currentTypeStart);
                    
                    // Create current based on type
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
                    
                    size_t paramsStart = json.find("\"params\": \"", currentTypeEnd) + 11;
                    size_t paramsEnd = json.find("\"", paramsStart);
                    if (paramsStart != std::string::npos && paramsEnd != std::string::npos) {
                        std::string params = json.substr(paramsStart, paramsEnd - paramsStart);
                        current->DeserializeParameters(params);
                    }
                    
                    synapse->AddCurrent(current);
                    
                    currentPos = json.find("}", currentTypeEnd) + 1;
                    if (json.find("]", currentPos) < json.find("{\n          \"name\":", currentPos)) break;
                }
            }
            
            pos = nameEnd;
            if (json.find("\"electrodes\":", pos) < json.find("{\n      \"name\":", pos)) break;
        }
    }
    
    // Parse electrodes with owners
    pos = json.find("\"electrodes\":");
    if (pos != std::string::npos) {
        while ((pos = json.find("{\n      \"name\":", pos)) != std::string::npos) {
            size_t nameStart = json.find("\"", pos + 15) + 1;
            size_t nameEnd = json.find("\"", nameStart);
            std::string name = json.substr(nameStart, nameEnd - nameStart);
            
            size_t typeStart = json.find("\"type\": \"", nameEnd) + 9;
            size_t typeEnd = json.find("\"", typeStart);
            std::string type = json.substr(typeStart, typeEnd - typeStart);
            
            std::string ownerName;
            size_t ownerPos = json.find("\"owner\": \"", typeEnd);
            if (ownerPos != std::string::npos && ownerPos < json.find("}", typeEnd)) {
                size_t ownerStart = ownerPos + 10;
                size_t ownerEndPos = json.find("\"", ownerStart);
                ownerName = json.substr(ownerStart, ownerEndPos - ownerStart);
            }
            
            TElectrode* electrode = network->AddElectrode(std::wstring(type.begin(), type.end()), 
                                                         std::wstring(name.begin(), name.end()), L"", 0, 0);
            
            // Set owner
            if (!ownerName.empty()) {
                auto& cells = network->GetCells();
                auto ownerIt = cells.find(std::wstring(ownerName.begin(), ownerName.end()));
                if (ownerIt != cells.end()) {
                    electrode->SetOwner(ownerIt->second.get());
                }
            }
            
            pos = nameEnd;
            if (pos >= json.length() - 10) break;
        }
    }
    
    return true;
}