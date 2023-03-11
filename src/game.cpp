#include "game.h"
#include <GL/glew.h>
#include "graphics\renderer.h"
#include "graphics\shader.h"

namespace LEAF {

    void InitializeGame(GameState& gameState)
    {
        gameState.running = true;
        ShaderSource vertex;
        vertex.ShaderCode = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
        
        ShaderSource fragment;
        fragment.ShaderCode = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                            "}\n\0";

        gameState.programSource = CreateShaderProgramSource(vertex, fragment);
        gameState.program = CreateShaderProgram(&gameState.programSource);
    }
};