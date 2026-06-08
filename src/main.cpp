/**
 * @file main.cpp
 * @brief Entry point for the Zomwave-2D game.
 *
 * Initialises the game object and runs the main loop.
 * On the Web platform, also provides browser-dimension query functions.
 */

#include "Game.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_WEB)
/**
 * @brief Returns the browser window's inner width.
 * @return Width in pixels.
 */
EM_JS(int, getBrowserWidth, (), { return window.innerWidth; });

/**
 * @brief Returns the browser window's inner height.
 * @return Height in pixels.
 */
EM_JS(int, getBrowserHeight, (), { return window.innerHeight; });
#endif

/**
 * @brief Application entry point.
 * @return 0 on successful exit.
 */
int main() {
    Game game; // create game loop object

    game.Run();
    return 0;
}
