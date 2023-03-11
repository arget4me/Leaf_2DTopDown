#pragma once
#include "graphics\shader.h"

namespace LEAF {

struct GameState {
    bool running;

    ShaderProgramSource programSource;
    ShaderProgram program;
};


void InitializeGame(GameState& gameState);

};
