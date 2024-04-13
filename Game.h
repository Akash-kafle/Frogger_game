#pragma once
#include <SDL.h>
#include <stdio.h>
#include "player.h"

// Constants for screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_WIDTH = 10;
const int PLAYER_HEIGHT = 10;

// Game class
class Game {
public:
    // Constructor to initialize the game with the given title, width, and height
    Game(const char* title, int width, int height);

    // Destructor to clean up the game resources
    ~Game();

    // Method to run the game
    void run();

private:

    // SDL window
    SDL_Window* window;

    // SDL surface contained by the window
    SDL_Surface* screenSurface;

    // SDL renderer
    SDL_Renderer* gRenderer;  

    // Player object
    Player player; 

    // Method to initialize SDL
    bool initializeSDL();

    // Method to create the window
    bool createWindow(const char* title, int width, int height);

    // Method to fill the surface with a specified color (white in this case)
    void fillSurface(Uint32 color);

    // Method to handle the event loop
    void eventLoop(Player,SDL_Renderer*);

    // Method to clean up the game resources
    void cleanup();
};


