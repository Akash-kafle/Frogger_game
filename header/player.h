#pragma once
#include <SDL.h>
#include <string>

class Player {
    int score;
    int X_position;
    int Y_position;
    bool alive;
    std::string imageFilename  = "frogger Character.bmp";  // Filename of the image

public:
    int Get_X_position();
    int Get_Y_position();
    int Get_score();
    bool Get_Alive();
    SDL_Texture* Get_Image(SDL_Renderer* gRenderer);  // Getter for the image
    void Update(SDL_KeyboardEvent);
};