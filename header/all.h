#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;
const int FPS = 60;
const int NUM_LANES = 3;
const int NUM_VEHICLES_PER_LANE = 7;  


// Define player and object sizes
const int PLAYER_WIDTH = 35;
const int PLAYER_HEIGHT = 35;

// Define object speeds
const int CAR_SPEED = 1;



//*****************************************************************************           Vehicles

class Vehicle {
public:
    // Constructor
    Vehicle(int startX, int startY, int speed, SDL_Texture* vehicleTexture);

    // Methods
    void update();
    void reset();
    void render(SDL_Renderer* renderer) const;

    // Getters and setters
    SDL_Rect getRect() const;
    int getX() const;
    int getY() const;
    int getSpeed() const;
    void setSpeed(int newSpeed);
    void moveLeft() {
        vehicleRect.x -= speed;
    }
    int getWidth() const {
        return width;
    }

    // Method to get the height of the vehicle/log
    int getHeight() const {
        return height;
    }

    SDL_Texture* getTexture() const {
        return cartexture;
    }
   

private:
    // Member variables
    int width;  // Width of the vehicle/log
    int height; // Height of the vehicle/log
    int speed;
    int initialX;
    int initialY;
    SDL_Rect vehicleRect;
    SDL_Texture* cartexture;
};

//**************************************************************************************************                  logs and water


class LogsWater {
public:
    // Constructor
    LogsWater(int startY, SDL_Texture* logTexture, SDL_Texture* waterTexture, int speed, int numLogs);
    LogsWater(int startY, SDL_Texture* logTexture, SDL_Texture* waterTexture, int speed);
    std::vector<Vehicle> logs;
    // Methods
    void update();
    void render(SDL_Renderer* renderer) const;
    void initializeLogs();

    int getY() const;
    int getWidth() const {
        return width;
    }

    // Method to get the height of the vehicle/log
    int getHeight() const {
        return height;
    }

    static SDL_Texture* getTexture() {
        return logTexture;
    }
    

private:
    
    // Member variables
    int width;  // Width of the vehicle/log
    int height; // Height of the vehicle/log
    int numLogs; 
    int yPosition;
    static SDL_Texture* logTexture;
    SDL_Texture* waterTexture;
    int logSpeed;
    
};


//**************************************************************************************                          Main game

class Game {
public:
    // Constructor
    Game();

    // Destructor
    ~Game();

    // Game initialization and running methods
    bool initialize(const char* title, int width, int height);
    void run();
    // Method to initialize vehicle lanes
    void initializeVehicleLanes();

    // Method to update vehicle lanes
    void updateVehicleLanes();
    void renderLogs(SDL_Renderer* renderer);

    // Method to render vehicle lanes
    void renderVehicleLanes(SDL_Renderer* renderer);
    void renderVehicles(SDL_Renderer* renderer);
    void renderFrog(SDL_Renderer* renderer);
    static SDL_Texture* carTexture;


private:
    // Initialization methods
    void initializeLogWaterLanes();
    bool loadResources();
    void renderVandF();

    // Game loop methods
    void handleInput(bool& running);
    void update();
    void render();
    void cleanup();
    bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);
    void renderLogs();
    

    // Member variables
    SDL_Window* window;
    SDL_Renderer* renderer ;
    SDL_Texture* frogTexture;
    
    SDL_Texture* shortLogTexture;
    SDL_Texture* mediumLogTexture;
    SDL_Texture* bigLogTexture;
    SDL_Texture* waterTexture;
    SDL_Texture* backgroundTexture;

    int frogX;
    int frogY;
    bool gameOver;

    // Lanes of logs/water and vehicles
    std::vector<LogsWater> logsWaterLanes;
    std::vector<std::vector<Vehicle>> vehicleLanes;
};
