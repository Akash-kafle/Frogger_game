#include "Game.h"
#include "player.h"


// Constructor
Game::Game(const char* title, int width, int height) : window(NULL), screenSurface(NULL) {
    if (initializeSDL()) {
        if (createWindow(title, width, height)) {
            screenSurface = SDL_GetWindowSurface(window);
        }
    }
}

// Destructor
Game::~Game() {
    cleanup();
}

// Initialize SDL
bool Game::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

// Create the window
bool Game::createWindow(const char* title, int width, int height) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

// Fill the surface with the specified color
void Game::fillSurface(Uint32 color) {
    if (screenSurface != NULL) {
        SDL_FillRect(screenSurface, NULL, color);
        SDL_UpdateWindowSurface(window);
    }
}

// Handle the event loop
void Game::eventLoop(Player player,SDL_Renderer* gRenderer) {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the current rendering
        SDL_RenderClear(gRenderer);

        // Render the player
        SDL_Texture* playerImage = player.Get_Image(gRenderer);
        SDL_Rect renderQuad = { player.Get_X_position(), player.Get_Y_position(), PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopy(gRenderer, playerImage, NULL, &renderQuad);

        // Update the screen
        SDL_RenderPresent(gRenderer);
    }
}

// Run the game
void Game::run() {
    fillSurface(SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF)); // Fill the surface white
    eventLoop(); // Start the event loop
}

// Clean up the game resources
void Game::cleanup() {
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}