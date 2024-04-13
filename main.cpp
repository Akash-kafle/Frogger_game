/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include "Game.h"

// Main function
int main(int argc, char* args[]) {
    // Create the game instance
    Game game("Frogger", SCREEN_WIDTH, SCREEN_HEIGHT);

    // Run the game
    game.run();

    // Cleanup will be called automatically when the game instance is destroyed
    return 0;
}
