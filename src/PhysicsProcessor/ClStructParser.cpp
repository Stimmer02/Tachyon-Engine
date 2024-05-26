#include "ClStructParser.h"

ClStructParser::ClStructParser(MacroManager* macroManager){
    this->macroManager = macroManager;
    error = "";
}

engineStruct* ClStructParser::processStruct(std::string structCode){
    std::vector<engineStruct::field> tempFields;
    engineStruct* structure = new engineStruct();

    structure->rawCode = structCode;

    long openningBracket = structCode.find('{');
    if (openningBracket == std::string::npos){
        error += "ERR: ClStructParser::processStruct could not find oppening bracket\n";
        delete structure;
        return nullptr;
    }

    long wordEnd = openningBracket;
    for (; structCode[wordEnd] == ' '; wordEnd--){
        if (wordEnd == 0){
            error += "ERR: ClStructParser::processStruct could not find structure name end\n";
            delete structure;
            return nullptr;
        }
    }

    long wordStart = wordEnd;
    for (; structCode[wordStart] != ' '; wordStart--){
        if (wordStart == 0){
            error += "ERR: ClStructParser::processStruct could not find structure name beginning\n";
            delete structure;
            return nullptr;
        }
    }
    wordStart += 1;
    structure->name = structCode.substr(wordStart, wordEnd-wordStart);

    for (long i = openningBracket + 1; i < structCode.length(); i++){

        if (structCode[i] == '/'){
            //comment
            i = structCode.find_first_of('\n', i);
        } else if (structCode[i] == '}'){
            //closing bracket
            break;
        } else if (structCode[i] != ' ' && structCode[i] != '\t' && structCode[i] != '\n'){
            //the beginning of field declaration
            wordStart = i;
            //variable type
            i = structCode.find_first_of(' ', i);
            std::string fieldType = structCode.substr(wordStart, i - wordStart);
            if (fieldType == "unsigned"){
                i = structCode.find_first_not_of(' ', i);
                wordStart = i;
                i = structCode.find_first_of(' ', i);

                fieldType += ' ' + structCode.substr(wordStart, i - wordStart);
            }
            bool pointer = false;
            if (fieldType[fieldType.length() - 1] == '*'){
                pointer = true;
                fieldType.erase(fieldType.length() - 1);
            }


            engineStruct::field field;
            field.type = stringToClType(fieldType.c_str());
            if (field.type == engineStruct::cl_invalid){
                // if (pointer){
                //     std::printf("pointer, %s\n", fieldType.c_str());
                // }
                error += "ERR: ClStructParser::processStruct\"" + fieldType + "\" does not name any known type\n";
                delete structure;
                return nullptr;
            } if (field.type == engineStruct::cl_struct){
                i = structCode.find_first_not_of(' ', i);
                wordStart = i;
                i = structCode.find_first_of(' ', i);
                fieldType = structCode.substr(wordStart, i - wordStart);

                if (fieldType[fieldType.length() - 1] == '*'){
                    pointer = true;
                    fieldType.erase(fieldType.length() - 1);
                }

                field.subStructName = fieldType;
            } else {
                field.subStructName.clear();
            }

            //variable name
            i = structCode.find_first_not_of(' ', i);
            if (structCode[i] == '*'){
                pointer = true;
                i++;
                i = structCode.find_first_not_of(' ', i);
            }
            wordStart = i;
            while (structCode[i] != ';' && structCode[i] != ' ' && structCode[i] != '='){
                i++;
                if (i == structCode.length()){
                    //err
                    delete structure;
                    return nullptr;
                }
            }
            bool valueSet = false;
            if (structCode[i] == '='){
                valueSet = true;
            }
            long nameEnd = i;
            field.name = structCode.substr(wordStart, i - wordStart).c_str();
            if (field.name[field.name.length() - 1] == ' '){
                field.name.erase(field.name.length() - 1);
            }

            if (structCode[i] == ' '){
                i = structCode.find_first_not_of(' ', i);
            }
            if (structCode[i] == '='){
                valueSet = true;
            }

            if (structCode[i] != ';'){
                i = structCode.find_first_of(';', i);
            }
            i++;
            i = structCode.find_first_not_of(' ', i);
            if (pointer){
                if (structCode[i] == '/'){
                    wordStart = i;
                    i = structCode.find_first_of(' ', i);
                    std::string word = structCode.substr(wordStart, i - wordStart).c_str();
                    if (word == "//ALLOCATE"){
                        i = structCode.find_first_not_of(' ', i);
                        wordStart = i;
                        while (structCode[i] != ' ' && structCode[i] != '\n'){
                            i++;
                            if (i == structCode.length()){
                                //err
                                delete structure;
                                return nullptr;
                            }
                        }
                        word = structCode.substr(wordStart, i - wordStart).c_str();
                        const float* macro = macroManager->getMacro(word);
                        if (macro == nullptr){
                            try {
                                field.arrSize = std::stoi(word);
                            } catch (const std::invalid_argument&){
                                error += "ERR: ClStructParser::processStruct could not parse \"" + word + "\" as integer\n";
                                delete structure;
                                return nullptr;
                            } catch (const std::out_of_range&){
                                error += "ERR: ClStructParser::processStruct could not parse \"" + word + "\" as integer\n";
                                delete structure;
                                return nullptr;
                            }
                        } else {
                            field.arrSize = *macro;
                        }
                    } else {
                        field.arrSize = 1;
                    }
                } else {
                    field.arrSize = 1;
                }
            } else {
                field.arrSize = 0;

                if (structCode[i] == '/'){
                    wordStart = i;
                    i = structCode.find_first_of(' ', i);
                    std::string word = structCode.substr(wordStart, i - wordStart).c_str();
                    if (word == "//SET"){
                        if (valueSet){
                            error += "WARN: ClStructParser::processStruct value already set, ignoring macro\n";
                        } else {
                            float valueTOSet;
                            i = structCode.find_first_not_of(' ', i);
                            wordStart = i;
                            while (structCode[i] != ' ' && structCode[i] != '\n'){
                                i++;
                                if (i == structCode.length()){
                                    //err
                                    delete structure;
                                    return nullptr;
                                }
                            }
                            
                            word = structCode.substr(wordStart, i - wordStart).c_str();
                            const float* macro = macroManager->getMacro(word);
                            if (macro == nullptr){
                                try {
                                    valueTOSet = std::stof(word);
                                } catch (const std::invalid_argument&){
                                    error += "ERR: ClStructParser::processStruct could not parse \"" + word + "\" as float\n";
                                    delete structure;
                                    return nullptr;
                                } catch (const std::out_of_range&){
                                    error += "ERR: ClStructParser::processStruct could not parse \"" + word + "\" as float\n";
                                    delete structure;
                                    return nullptr;
                                }
                            } else {
                                valueTOSet = *macro;
                            }
                            std::string toInsert;
                            if (field.type == engineStruct::cl_float || field.type == engineStruct::cl_double || field.type == engineStruct::cl_half){
                                toInsert = " = " + std::to_string(valueTOSet);
                            } else {
                                toInsert = " = " + std::to_string(int(valueTOSet));
                            }
                            structCode.insert(nameEnd, toInsert);
                            i += toInsert.length();

                            field.defaultValue = valueTOSet;
                        }
                    }
                }
            }
            i = structCode.find_first_of('\n', i);
            field.subStruct = nullptr;
            tempFields.push_back(field);
        }
    }

    structure->fieldCount = tempFields.size();
    structure->fields = new engineStruct::field[tempFields.size()];
    for (uint i = 0; i < tempFields.size(); i++){
        structure->fields[i] = tempFields.at(i);
    }

    // std::printf("STRUCT CODE:\n%s\n", structCode.c_str());
    return structure;
}



engineStruct::cl_type ClStructParser::stringToClType(const char* word){
     struct cmp_str{
        bool operator()(const char* a, const char* b) const{
            return std::strcmp(a, b) < 0;
        }
    };
    static std::map<const char*, engineStruct::cl_type, cmp_str> clTypeMap{
        {"bool",           engineStruct::cl_bool},
        {"char",           engineStruct::cl_char},
        {"uchar",          engineStruct::cl_uchar},
        {"unsigned char",  engineStruct::cl_uchar},
        {"short",          engineStruct::cl_short},
        {"ushort",         engineStruct::cl_ushort},
        {"unsigned short", engineStruct::cl_ushort},
        {"int",            engineStruct::cl_int},
        {"uint",           engineStruct::cl_uint},
        {"unsigned int",   engineStruct::cl_uint},
        {"long",           engineStruct::cl_long},
        {"ulong",          engineStruct::cl_ulong},
        {"unsigned long",  engineStruct::cl_ulong},
        {"half",           engineStruct::cl_half},
        {"float",          engineStruct::cl_float},
        {"double",         engineStruct::cl_double},
        {"size_t",         engineStruct::cl_size_t},
        {"void",           engineStruct::cl_void},
        {"struct",         engineStruct::cl_struct},
        {"__struct",         engineStruct::cl_struct},
        {"__struct__",       engineStruct::cl_struct},
    };

    try {
        return clTypeMap.at(word);
    } catch (std::out_of_range const&){
        return engineStruct::cl_invalid;
    }

    return engineStruct::cl_invalid;
}



