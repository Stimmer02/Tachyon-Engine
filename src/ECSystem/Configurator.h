#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <unordered_map>

class Configurator {

private:

    std::unordered_map<std::string, std::string> configMap;

public:

    Configurator(const std::string & filePath);

    void ParseInt(const std::string & varName, int & value, const int & defaultValue = 0) const;

    void ParseFloat(const std::string & varName, float & value, const float & defaultValue = 0.0f) const;

    void ParseDouble(const std::string & varName, double & value, const double & defaultValue = 0.0) const;

    void ParseString(const std::string & varName, std::string & value, const std::string & defaultValue = "null") const;

    void ParseBoolean(const std::string & varName, bool & value, const bool & defaultValue = false) const;

};

#endif
