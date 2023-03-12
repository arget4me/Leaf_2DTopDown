#pragma once

#include "graphics\renderer.h"
#include "graphics\shader.h"
#include "memory\memorymanager.h"

namespace LEAF {

struct GameState {
    bool running;

    Renderer renderer;
    MemoryManager persistentMemory;
    MemoryManager temporaryMemory;
};


void InitializeGame(GameState& gameState);
void UpdateAndRenderGame(GameState& gameState);

};
