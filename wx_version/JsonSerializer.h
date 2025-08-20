#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <string>
#include <fstream>

class TNetwork;

class JsonSerializer
{
public:
    static bool SaveNetwork(const TNetwork* network, const std::string& filename);
    static bool LoadNetwork(TNetwork* network, const std::string& filename);

private:
    static std::string NetworkToJson(const TNetwork* network);
    static bool JsonToNetwork(const std::string& json, TNetwork* network);
};

#endif