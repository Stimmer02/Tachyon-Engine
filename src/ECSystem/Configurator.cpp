#include "Configurator.h"


Configurator::Configurator(const std::string& filePath) {
    std::ifstream file(filePath);

    assert( file.is_open() && "Unable to open configuration file");

    std::string line;
    std::string key;
    std::string value;

    while (std::getline(file, line)) {

        std::istringstream is_line(line);

        if ( std::getline(is_line, key, '=') ) {

            if (std::getline(is_line, value))
                configMap[key] = value;

        }

    }
}

void Configurator::ParseInt(const std::string& varName, int& value, const int & defaultValue) const {

    auto it = configMap.find(varName);

    if (it != configMap.end()) {

        try {
            value = std::stoi(it->second);
        } catch (...) {
            value = defaultValue;
        }

    } else {
        value = defaultValue;
    }

}

void Configurator::ParseFloat(const std::string & varName, float & value, const float & defaultValue) const {

    auto it = configMap.find(varName);

    if (it != configMap.end()) {

        try {
            value = std::stof(it->second);
        } catch (...) {
            value = defaultValue;
        }

    } else {
        value = defaultValue;
    }
}


void Configurator::ParseDouble(const std::string & varName, double & value, const double & defaultValue) const {

    auto it = configMap.find(varName);

    if (it != configMap.end()) {

        try {
            value = std::stod(it->second);
        } catch (...) {
            value = defaultValue;
        }

    } else {
        value = defaultValue;
    }
}

void Configurator::ParseString(const std::string & varName, std::string & value, const std::string & defaultValue) const {
    auto it = configMap.find(varName);

    if (it != configMap.end()) {
        value = it->second;
    } else {
        value = defaultValue;
    }
}

void Configurator::ParseBoolean(const std::string & varName, bool & value, const bool & defaultValue) const {

    auto it = configMap.find(varName);

    if (it != configMap.end()) {
        std::string val = it->second;
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);

        if (val == "1" || val == "true") {
            value = true;
        } else if (val == "0" || val == "false") {
            value = false;
        } else {
            value = defaultValue;
        }

    } else {
        value = defaultValue;
    }
}
