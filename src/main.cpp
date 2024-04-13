/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO
//#include "Game.h"
#include "all.h"

// Main function
int main(int argc, char* args[]) {
    Game game;

    // Initialize the game
    bool checker = game.initialize("Frogger", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!checker) {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    // Run the game
    game.run();

    return 0;
}
