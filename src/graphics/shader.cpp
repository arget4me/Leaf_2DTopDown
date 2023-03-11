#include "shader.h"
#include <gl/glew.h>

namespace LEAF{

ShaderProgramSource CreateShaderProgramSource(ShaderSource vertex, ShaderSource fragment, ShaderSource tesselation, ShaderSource geometry)
{
    return {vertex, fragment, tesselation, geometry};
}

GLuint LoadShader(std::string source, GLenum type)
{
    if(source.empty())
    {
        return 0;
    }

    // Create and load the shader source
    GLuint shader = glCreateShader(type);
    if(shader == 0)
    {
        return 0;
    }
    const char* _source = source.c_str();
    glShaderSource(shader, 1, &_source, NULL);

    // Compile and check status
    GLint compiled;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        // TODO: Print error code here
        // if(infoLen > 1)
        // {
        //      glGetShaderInfoLog(shader, infoLen, NULL, infoLogBuffer);
        // }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

ShaderProgram CreateShaderProgram(ShaderProgramSource* source)
{
    if(!source)
    {
        return {nullptr, 0};
    }

    GLuint vertexShader = LoadShader(source->vertex.ShaderCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShader(source->fragment.ShaderCode.c_str(), GL_FRAGMENT_SHADER);
    GLuint tesselationShader = LoadShader(source->tesselation.ShaderCode.c_str(), GL_TESS_CONTROL_SHADER);
    GLuint geometryShader = LoadShader(source->geometry.ShaderCode.c_str(), GL_GEOMETRY_SHADER);

    GLuint shaderProgram = glCreateProgram();
    if(shaderProgram == 0)
    {
        return {nullptr, 0};
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, tesselationShader);
    glAttachShader(shaderProgram, geometryShader);

    glLinkProgram(shaderProgram);

    GLint isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);

		// ERROR_LOG("shader link failed: " << shader.vertex_source_path << ", " << shader.fragment_source_path <<"\n");
		// DEBUG_LOG(errorLog << "\n");
		return {nullptr, 0};
	}

    return {source, shaderProgram};
}

};