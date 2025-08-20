#include "JsonSerializer.h"
#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include "RT_Synapse_wx.h"
#include "RT_Electrode_wx.h"
#include <sstream>

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
    std::ostringstream json;
    json << "{\n";
    json << "  \"name\": \"" << std::string(network->Name().begin(), network->Name().end()) << "\",\n";
    json << "  \"cells\": [\n";
    
    bool first = true;
    for (const auto& pair : network->GetCells()) {
        if (!first) json << ",\n";
        const TCell* cell = pair.second.get();
        json << "    {\n";
        json << "      \"name\": \"" << std::string(cell->Name().begin(), cell->Name().end()) << "\",\n";
        json << "      \"type\": \"" << std::string(cell->ClassKey().begin(), cell->ClassKey().end()) << "\",\n";
        json << "      \"x\": " << cell->GetX() << ",\n";
        json << "      \"y\": " << cell->GetY() << ",\n";
        json << "      \"active\": " << (cell->IsActive() ? "true" : "false") << "\n";
        json << "    }";
        first = false;
    }
    
    json << "\n  ],\n";
    json << "  \"synapses\": [\n";
    
    first = true;
    for (const auto& pair : network->GetSynapses()) {
        if (!first) json << ",\n";
        const TSynapse* synapse = pair.second.get();
        json << "    {\n";
        json << "      \"name\": \"" << std::string(synapse->Name().begin(), synapse->Name().end()) << "\",\n";
        json << "      \"type\": \"" << std::string(synapse->ClassKey().begin(), synapse->ClassKey().end()) << "\",\n";
        json << "      \"pre\": \"" << (synapse->Pre() ? std::string(synapse->Pre()->Name().begin(), synapse->Pre()->Name().end()) : "") << "\",\n";
        json << "      \"post\": \"" << (synapse->Post() ? std::string(synapse->Post()->Name().begin(), synapse->Post()->Name().end()) : "") << "\",\n";
        json << "      \"active\": " << (synapse->IsActive() ? "true" : "false") << "\n";
        json << "    }";
        first = false;
    }
    
    json << "\n  ],\n";
    json << "  \"electrodes\": [\n";
    
    first = true;
    for (const auto& pair : network->GetElectrodes()) {
        if (!first) json << ",\n";
        const TElectrode* electrode = pair.second.get();
        json << "    {\n";
        json << "      \"name\": \"" << std::string(electrode->Name().begin(), electrode->Name().end()) << "\",\n";
        json << "      \"type\": \"" << std::string(electrode->ClassKey().begin(), electrode->ClassKey().end()) << "\",\n";
        json << "      \"owner\": \"" << (electrode->Owner() ? std::string(electrode->Owner()->Name().begin(), electrode->Owner()->Name().end()) : "") << "\",\n";
        json << "      \"active\": " << (electrode->IsActive() ? "true" : "false") << "\n";
        json << "    }";
        first = false;
    }
    
    json << "\n  ]\n";
    json << "}";
    
    return json.str();
}

bool JsonSerializer::JsonToNetwork(const std::string& json, TNetwork* network)
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