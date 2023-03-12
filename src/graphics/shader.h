#pragma once

#include <string>
#include <GL/glew.h>

namespace LEAF {

struct ShaderSource {
    std::string ShaderCode;
    GLuint id;
};

struct ShaderProgramSource {
    ShaderSource vertex;
    ShaderSource fragment;
    ShaderSource tesselation;
    ShaderSource geometry;
};

struct ShaderProgram {
    ShaderProgramSource* source;
    GLuint id;
};

// You can combine shader types but it's only meant to be used in combination of array. 
// If multiple are combined the first type will be used.
enum class ShaderUniformType : uint8_t {
    I32   =  1, 
    U32   =  2,
    I64   =  4,
    U64   =  8,
    R32   = 16,
    R64   = 32,
    Array = 64,
};

struct ShaderUniform {
    std::string name;
    uint8_t     type;
    union value {
        int32_t   I32;
        uint32_t  U32;
        float     F32;

        int64_t   I64;
        uint64_t  U64;
        double    F64;

        struct array {
            uint32_t size;
            void*    data;
        };
    };
};

ShaderProgramSource CreateShaderProgramSource(ShaderSource vertexFilepath, ShaderSource fragmentFilepath, ShaderSource tesselationFilepath = {}, ShaderSource geometryFilepath = {});
ShaderProgram CreateShaderProgram(ShaderProgramSource* source);

};