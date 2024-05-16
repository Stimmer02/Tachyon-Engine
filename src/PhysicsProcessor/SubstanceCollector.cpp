#include "SubstanceCollector.h"

SubstanceCollector::SubstanceCollector(){
    error = "";
    idCount = 1;
}

char SubstanceCollector::parseConfig(std::string path){
    std::ifstream file(path);
    if (!file.is_open()){
        error += "ERR SubstanceCollector::parseConfig: Failed to open file: " + path + "\n";
        return 1;
    }

    std::string substancesFolder = "";
    std::string propertiesFolder = "";

    std::vector<std::string> substancesFilesNames;
    std::vector<std::string> propertiesFilesNames;

    std::string line;

    while(std::getline(file, line)){
        if (line.empty() || line[0] == '#' || line[0] == ' '){
            continue;
        }

        size_t position = line.find("=");
        if (position != std::string::npos){
            std::string key = line.substr(0, position);
            std::string value = line.substr(position + 1);
             if (key == "SUBSTANCES_FOLDER"){
                substancesFolder = value;
            } else if (key == "PROPERTIES_FOLDER"){
                propertiesFolder = value;
            } else {
                error += "ERR SubstanceCollector::parseConfig: Invalid key: " + key + "=\n";
                return 1;
            }
            continue;
        }

        position = line.find(":");
        if (position != std::string::npos){
            std::string key = line.substr(0, position);

            while(std::getline(file, line)){
                if (line.empty() || line[0] == '#' || line[0] == ' '){
                    break;
                }
                size_t commaPosition = line.find(",");
                std::string value;
                if (commaPosition != std::string::npos){
                    value = line.substr(0, commaPosition);
                } else {
                    value = line;
                }

                if (key == "SUBSTANCE_FILE_LIST"){
                    substancesFilesNames.push_back(value);
                } else if (key == "PROPERTY_FILE_LIST"){
                    propertiesFilesNames.push_back(value);
                } else {
                    error += "ERR SubstanceCollector::parseConfig: Invalid key: " + key + "\n";
                    return 1;
                }

                if (commaPosition == std::string::npos){
                    break;
                }
            }
        }
    }

    file.close();

    if (substancesFolder[substancesFolder.size() - 1] != '/'){
        substancesFolder += '/';
    }

    if (propertiesFolder[propertiesFolder.size() - 1] != '/'){
        propertiesFolder += '/';
    }

    for (std::string fileName : propertiesFilesNames){
        if (loadPropertiesFromFile(propertiesFolder + fileName) != 0){
            error += "ERR SubstanceCollector::parseConfig: Failed to load phroperties from file: " + propertiesFolder + '/' + fileName + "\n";
            return 1;
        }
    }

    for (std::string fileName : substancesFilesNames){
        if (loadSubstancesFromFile(substancesFolder + fileName) != 0){
            error += "ERR SubstanceCollector::parseConfig: Failed to load substances from file: " + substancesFolder + '/' + fileName + "\n";
            return 1;
        }
    }

    return 0;
}

char SubstanceCollector::loadPropertiesFromFile(std::string path){
    std::ifstream file(path);
    if (!file.is_open()){
        error += "ERR SubstanceCollector::loadPropertiesFromFile: Failed to open file: " + path + "\n";
        return 1;
    }

    std::string line;
    substanceField phroperty;

    bool foundName = false;
    bool foundType = false;
    bool foundDefaultValue = false;

    while (std::getline(file, line)){
        if (line.empty() || line[0] == '#' || line[0] == ' '){
            continue;
        }

        size_t position = line.find("=");
        if (position == std::string::npos){
            continue;
        }

        std::string key = line.substr(0, position);

        if (key == "NAME"){
            std::string name = line.substr(position + 1);
            if (foundName){
                error += "ERR SubstanceCollector::loadPropertiesFromFile: Name already found (" + phroperty.name + " and " + name + ")\n";
                return 1;
            }
            phroperty.name = name;
            foundName = true;
        } else if (key == "TYPE"){
            std::string type = line.substr(position + 1);
            if (foundType){
                error += "ERR SubstanceCollector::loadPropertiesFromFile: Type already found (" + type + ")\n";
                return 1;
            }
            phroperty.type = stringToClType(type);
            if (phroperty.type == engineStruct::cl_type::cl_invalid){
                error += "ERR SubstanceCollector::loadPropertiesFromFile: Invalid type: " + type + "\n";
                return 1;
            }
            foundType = true;
        } else if (key == "DEFAULT"){
            float defaultValue;
            try{
                defaultValue = std::stof(line.substr(position + 1));
            } catch (std::invalid_argument){
                error += "ERR SubstanceCollector::loadPropertiesFromFile: Invalid default value: " + line.substr(position + 1) + "\n";
                return 1;
            }

            if (foundDefaultValue){
                error += "ERR SubstanceCollector::loadPropertiesFromFile: Default value already found (" + std::to_string(phroperty.defaultValue) + " and " + std::to_string(defaultValue) + ")\n";
                return 1;
            }
            phroperty.defaultValue = defaultValue;
            foundDefaultValue = true;
        }

        if (foundName && foundType && foundDefaultValue){
            bool alreadyExists = false;
            for (uint i = 0; i < substancePhroperties.size(); i++){
                if (substancePhroperties[i].name == phroperty.name){
                    substancePhroperties[i] = phroperty;
                    alreadyExists = true;
                }
            }

            foundName = false;
            foundType = false;
            foundDefaultValue = false;
            if (!alreadyExists){
                substancePhroperties.push_back(phroperty);
            }
        }
    }

    file.close();
    return 0;
}

char SubstanceCollector::loadSubstancesFromFile(std::string path){
    std::ifstream file(path);
    if (!file.is_open()){
        error += "ERR SubstanceCollector::loadSubstancesFromFile: Failed to open file: " + path + "\n";
        return 1;
    }

    std::string line;
    substance substance;

    while (std::getline(file, line)){
        if (line.empty() || line[0] == '#' || line[0] == ' '){
            continue;
        }

        size_t position = line.find(":");
        if (position != std::string::npos){
            substance.name = line.substr(0, position);
            setDefaultPhroperties(substance);
            std::string prevLine = line;
            while(std::getline(file, line)){
                if (line.empty() || line[0] == '#' || line[0] == ' '){
                    break;
                }

                size_t position = line.find("=");
                if (position == std::string::npos){
                    error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid line: " + line + "\n";
                    return 1;
                }

                std::string propertyName = line.substr(0, position);
                bool found = false;
                
                if (propertyName == "COLOR"){
                    std::string values = line.substr(position + 1);
                    position = values.find(",");
                    if (position == std::string::npos){
                        error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid color definition: " + line + "\n";
                        return 1;
                    }
                    std::string rColor = values.substr(0, position);
                    values = values.substr(position + 1);
                    position = values.find(",");
                    if (position == std::string::npos){
                        error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid color definition: " + line + "\n";
                        return 1;
                    }
                    std::string gColor = values.substr(0, position);
                    std::string bColor = values.substr(position + 1);
                    
                    try{
                        substance.R = std::stof(rColor);
                        substance.G = std::stof(gColor);
                        substance.B = std::stof(bColor);
                    } catch (std::invalid_argument){
                        error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid color definition: " + line + "\n";
                        return 1;
                    }

                    found = true;
                } else if (propertyName == "MOVABLE"){
                    std::string value = line.substr(position + 1);
                    if (value == "true" || value == "True" || value == "TRUE" || value == "1"){
                        substance.movable = true;
                    } else if (value == "false" || value == "False" || value == "FALSE" || value == "0"){
                        substance.movable = false;
                    } else {
                        error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid value: " + value + "\n";
                        return 1;
                    }
                    found = true;
                } else {
                    float value;
                    try{
                        value = std::stof(line.substr(position + 1));
                    } catch (std::invalid_argument){
                        error += "ERR SubstanceCollector::loadSubstancesFromFile: Invalid value: " + line.substr(position + 1) + "\n";
                        return 1;
                    }

                    for (uint i = 0; i < substancePhroperties.size(); i++){
                        if (substancePhroperties[i].name == propertyName){
                            substance.values[i] = value;
                            found = true;
                        }
                    }
                }

                if (!found){
                    error += "ERR SubstanceCollector::loadSubstancesFromFile: Phroperty not found: " + propertyName + "\n";
                    return 1;
                }
                prevLine = line;
            }

            bool alreadyExists = false;
            for (uint i = 0; i < substanceTable.size(); i++){
                if (substanceTable[i].name == substance.name){
                    substanceTable[i] = substance;
                    alreadyExists = true;
                }
            }

            if (!alreadyExists){
                giveIdToSubstances(substance);
                substanceTable.push_back(substance);
            }
        }
    }

    file.close();
    return 0;
}

std::string SubstanceCollector::createSubstanceClStruct(){
    std::string structString = "struct substance ((allinged)){\n";
    structString += "    struct color color;\n";
    structString += "    bool movable;\n";
    for (substanceField phroperty : substancePhroperties){
        std::string type = clTypeToString(phroperty.type);
        if (type == "invalid"){
            error += "ERR SubstanceCollector::createSubstanceClStruct: Invalid type: " + type + "\n";
            return "";
        }
        structString += "    " + type + ' ' + phroperty.name + ";\n";
    }
    structString += "};\n";
    return structString;
}

std::string SubstanceCollector::getError(){
    return error;
}

engineStruct::cl_type SubstanceCollector::stringToClType(std::string type){
    static std::map<std::string, engineStruct::cl_type> typeMap = {
        {"half", engineStruct::cl_type::cl_half},
        {"HALF", engineStruct::cl_type::cl_half},
        {"double", engineStruct::cl_type::cl_double},
        {"DOUBLE", engineStruct::cl_type::cl_double},
        {"float", engineStruct::cl_type::cl_float},
        {"FLOAT", engineStruct::cl_type::cl_float},
        {"char", engineStruct::cl_type::cl_char},
        {"CHAR", engineStruct::cl_type::cl_char},
        {"uchar", engineStruct::cl_type::cl_uchar},
        {"UCHAR", engineStruct::cl_type::cl_uchar},
        {"short", engineStruct::cl_type::cl_short},
        {"SHORT", engineStruct::cl_type::cl_short},
        {"ushort", engineStruct::cl_type::cl_ushort},
        {"USHORT", engineStruct::cl_type::cl_ushort},
        {"long", engineStruct::cl_type::cl_long},
        {"LONG", engineStruct::cl_type::cl_long},
        {"ulong", engineStruct::cl_type::cl_ulong},
        {"ULONG", engineStruct::cl_type::cl_ulong},
        {"size_t", engineStruct::cl_type::cl_size_t},
        {"SIZE_T", engineStruct::cl_type::cl_size_t},
        // {"ptrdiff_t", engineStruct::cl_type::cl_ptrdiff_t},
        // {"PTRDIFF_T", engineStruct::cl_type::cl_ptrdiff_t},
        // {"intptr_t", engineStruct::cl_type::cl_intptr_t},
        // {"INTPTR_T", engineStruct::cl_type::cl_intptr_t},
        // {"uintptr_t", engineStruct::cl_type::cl_uintptr_t},
        // {"UINTPTR_T", engineStruct::cl_type::cl_uintptr_t},
        // {"void", engineStruct::cl_type::cl_void},
        // {"VOID", engineStruct::cl_type::cl_void},
        // {"struct", engineStruct::cl_type::cl_struct},
        // {"STRUCT", engineStruct::cl_type::cl_struct},
        {"int", engineStruct::cl_type::cl_int},
        {"INT", engineStruct::cl_type::cl_int},
        {"uint", engineStruct::cl_type::cl_uint},
        {"UINT", engineStruct::cl_type::cl_uint},
        {"bool", engineStruct::cl_type::cl_bool},
        {"BOOL", engineStruct::cl_type::cl_bool},
        {"invalid", engineStruct::cl_type::cl_invalid},
        {"INVALID", engineStruct::cl_type::cl_invalid},
    };

    if (typeMap.find(type) == typeMap.end()){
        return engineStruct::cl_type::cl_invalid;
    }

    return typeMap[type];
}

std::string SubstanceCollector::clTypeToString(engineStruct::cl_type type){
    static std::map<engineStruct::cl_type, std::string> typeMap = {
        {engineStruct::cl_type::cl_half, "half"},
        {engineStruct::cl_type::cl_double, "double"},
        {engineStruct::cl_type::cl_float, "float"},
        {engineStruct::cl_type::cl_char, "char"},
        {engineStruct::cl_type::cl_uchar, "uchar"},
        {engineStruct::cl_type::cl_short, "short"},
        {engineStruct::cl_type::cl_ushort, "ushort"},
        {engineStruct::cl_type::cl_long, "long"},
        {engineStruct::cl_type::cl_ulong, "ulong"},
        {engineStruct::cl_type::cl_size_t, "size_t"},
        // {engineStruct::cl_type::cl_ptrdiff_t, "ptrdiff_t"},
        // {engineStruct::cl_type::cl_intptr_t, "intptr_t"},
        // {engineStruct::cl_type::cl_uintptr_t, "uintptr_t"},
        // {engineStruct::cl_type::cl_void, "void"},
        // {engineStruct::cl_type::cl_struct, "struct"},
        {engineStruct::cl_type::cl_int, "int"},
        {engineStruct::cl_type::cl_uint, "uint"},
        {engineStruct::cl_type::cl_bool, "bool"},
        {engineStruct::cl_type::cl_invalid, "invalid"},
    };

    if (typeMap.find(type) == typeMap.end()){
        return "invalid";
    }

    return typeMap[type];
}

void SubstanceCollector::setDefaultPhroperties(substance& substance){
    substance.values.clear();
    substance.values.resize(substancePhroperties.size());
    substance.R = 1;
    substance.G = 0;
    substance.B = 1;
    substance.movable = true;
    for (substanceField phroperty : substancePhroperties){
        substance.values.push_back(phroperty.defaultValue);
    }
}

void SubstanceCollector::giveIdToSubstances(substance& substance){
    substance.id = idCount;
    idCount++;
}