#include "raylib.h"
#include <cmath> // for sin and cos
#include <cstdlib>
#include "game_loop.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// used to get web width and height in browser
#if defined(PLATFORM_WEB)
EM_JS(int, getBrowserWidth, (), { return window.innerWidth; });

EM_JS(int, getBrowserHeight, (), { return window.innerHeight; });
#endif

// class used to set up game loop


int main() {
    GameLoop gameLoop; // create game loop object
    
    gameLoop.Run();
    return 0;
}
