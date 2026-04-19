#include "Game.h"
#include <iostream>
#include <vector>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// used to get web width and height in browser
#if defined(PLATFORM_WEB)
EM_JS(int, getBrowserWidth, (), { return window.innerWidth; });

EM_JS(int, getBrowserHeight, (), { return window.innerHeight; });
#endif

int main() {
    Game game; // create game loop object

    game.Run();
    return 0;
}
