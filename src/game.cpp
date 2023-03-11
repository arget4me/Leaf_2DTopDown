#include "game.h"
#include <GL/glew.h>
#include "graphics\renderer.h"
#include "graphics\shader.h"
#include "image\image.h"

namespace LEAF {

    void InitializeGame(GameState& gameState)
    {
        constexpr uint32_t TOTALSIZE = 128 * MEGABYTE;
        constexpr uint32_t TEMP_TOTALSIZE = 8 * MEGABYTE;
        constexpr uint32_t BLOCKSIZE = 64;
        constexpr uint32_t NUMBLOCKS = TOTALSIZE / BLOCKSIZE;
        constexpr uint32_t TEMP_NUMBLOCKS = TEMP_TOTALSIZE / BLOCKSIZE;
        gameState.persistentMemory = MemoryManager(BLOCKSIZE, NUMBLOCKS);
        gameState.temporaryMemory = MemoryManager(BLOCKSIZE, TEMP_NUMBLOCKS, true);
        Image debug_image = LoadImage("data/images/debug_image.png", gameState.persistentMemory);
        Image debug_image2 = LoadImage("data/images/debug_image.png", gameState.persistentMemory);
        Image debug_image3 = LoadImage("data/images/debug_image.png", gameState.persistentMemory);
        Image debug_image4 = LoadImage("data/images/debug_image.png", gameState.persistentMemory);
        Image debug_image5 = LoadImage("data/images/debug_image.png", gameState.persistentMemory);
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