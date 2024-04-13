#pragma once
#include <SDL.h>
#include <string>

class Player {
    int score;
    int X_position;
    int Y_position;
    bool alive;
    std::string imageFilename;  // Filename of the image

public:
    int Get_X_position();
    int Get_Y_position();
    int Get_score();
    bool Get_Alive();
    std::string Get_ImageFilename();  // Getter for the image filename
    void Set_ImageFilename(std::string newImageFilename);  // Setter for the image filename
    SDL_Texture* Get_Image(SDL_Renderer* gRenderer);  // Getter for the image
    void Update(SDL_KeyboardEvent);
};