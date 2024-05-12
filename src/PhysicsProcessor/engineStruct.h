#ifndef ENGINE_STRUCT_H
#define ENGINE_STRUCT_H

#include <string>

typedef unsigned char uchar;



struct engineStruct{
    enum cl_type{
        cl_bool,
        cl_char,
        cl_uchar,
        cl_short,
        cl_ushort,
        cl_int,
        cl_uint,
        cl_long,
        cl_ulong,
        cl_half,
        cl_float,
        cl_double,
        cl_size_t,
        cl_ptrdiff_t,
        cl_intptr_t,
        cl_uintptr_t,
        cl_void,
        cl_struct,
        cl_invalid
    };

    struct field{
        std::string name;
        cl_type type = cl_invalid;
        engineStruct* subStruct = nullptr;
        std::string subStructName;
        uint arrSize = 0;
        uchar byteSize = 0;
    };

    std::string name;
    std::string rawCode;
    uint byteSize = 0;
    ushort fieldCount = 0;
    field* fields = nullptr;
};

#endif