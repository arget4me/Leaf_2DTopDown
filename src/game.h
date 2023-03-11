#pragma once

#include "graphics\shader.h"
#include "memory\memorymanager.h"

namespace LEAF {

struct GameState {
    bool running;

    ShaderProgramSource programSource;
    ShaderProgram program;
    MemoryManager persistentMemory;
    MemoryManager temporaryMemory;
};


void InitializeGame(GameState& gameState);

};
