#include "SizeCalculator.h"

SizeCalculator::SizeCalculator(uint pointerSize): pointerSize(pointerSize){}


char SizeCalculator::calculate(engineStruct* structure){
    structure->byteSize = 0;
    for (uint i = 0; i < structure->fieldCount; i++){
        if (structure->fields[i].arrSize > 0){
            structure->fields[i].byteSize = pointerSize;
        } else if (structure->fields[i].type == engineStruct::cl_struct){
            if (structure->fields[i].subStruct == nullptr){
                error = "ERR: SizeCalculator::calculate substructure \"" + structure->fields[i].subStructName + "\" pointer of\"" + structure->name + "\" structure, points to nullptr";
                return -1;
            }
            if (calculate(structure->fields[i].subStruct)){
                return -2;
            }
            structure->fields[i].byteSize = structure->fields[i].subStruct->byteSize;
        } else {
            structure->fields[i].byteSize = clTypeSize(structure->fields[i].type);
        }
        structure->byteSize += structure->fields[i].byteSize;
    }
    return 0;
}

ushort SizeCalculator::clTypeSize(engineStruct::cl_type type){
    switch (type){
        case engineStruct::cl_bool:
            return sizeof(cl_char);
        case engineStruct::cl_char:
            return sizeof(cl_char);
        case engineStruct::cl_uchar:
            return sizeof(cl_uchar);
        case engineStruct::cl_short:
            return sizeof(cl_short);
        case engineStruct::cl_ushort:
            return sizeof(cl_ushort);
        case engineStruct::cl_int:
            return sizeof(cl_int);
        case engineStruct::cl_uint:
            return sizeof(cl_uint);
        case engineStruct::cl_long:
            return sizeof(cl_long);
        case engineStruct::cl_ulong:
            return sizeof(cl_ulong);
        case engineStruct::cl_half:
            return sizeof(cl_half);
        case engineStruct::cl_float:
            return sizeof(cl_float);
        case engineStruct::cl_double:
            return sizeof(cl_double);
        case engineStruct::cl_size_t:
            return sizeof(size_t);//?
        case engineStruct::cl_ptrdiff_t:
            return pointerSize;
        case engineStruct::cl_intptr_t:
            return pointerSize;
        case engineStruct::cl_uintptr_t:
            return pointerSize;
        case engineStruct::cl_void:
            return 0;
        case engineStruct::cl_struct:
            return 0;
        case engineStruct::cl_invalid:
            return 0;
    }
    return 0;
}
