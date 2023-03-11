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

ShaderProgramSource CreateShaderProgramSource(ShaderSource vertexFilepath, ShaderSource fragmentFilepath, ShaderSource tesselationFilepath = {}, ShaderSource geometryFilepath = {});
ShaderProgram CreateShaderProgram(ShaderProgramSource* source);

};