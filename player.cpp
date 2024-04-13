#pragma once
#include "player.h"
#include "Game.h"

int Player::Get_score() {
	return this->score;
}

int Player::Get_X_position() {
	return this->X_position;
}

int Player::Get_Y_position() {
	return this->Y_position;
}

bool Player::Get_Alive() {
    return this->alive;
}

void Player::Update(SDL_KeyboardEvent event) {
    switch (event.keysym.sym) {
    case SDLK_UP:
        Y_position = Y_position - 10 < 0 ? 0 : Y_position - 10;
        break;
    case SDLK_DOWN:
        Y_position = Y_position + 10 > SCREEN_HEIGHT ? SCREEN_HEIGHT : Y_position + 10;
        break;
    case SDLK_LEFT:
        X_position = X_position - 10 < 0 ? 0 : X_position - 10;
        break;
    case SDLK_RIGHT:
        X_position = X_position + 10 > SCREEN_WIDTH ? SCREEN_WIDTH : X_position + 10;
        break;
    default:
        break;
    }
}

std::string Player::Get_ImageFilename() {   // Getter for the image filename
    return this->imageFilename; 
} 

void Player::Set_ImageFilename(std::string newImageFilename) {  // Setter for the image filename
    this->imageFilename = newImageFilename; 
} 

SDL_Texture* Player::Get_Image(SDL_Renderer* gRenderer) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(imageFilename.c_str());
    SDL_Texture* image = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return image;
}