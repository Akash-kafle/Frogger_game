#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

// Constants for game properties
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_WIDTH = 35;
const int PLAYER_HEIGHT = 40;
const int MEDIUM_LOG_LENGTH = PLAYER_WIDTH * 2;
const int BIG_LOG_LENGTH = PLAYER_WIDTH * 3;
const int FPS = 60;
const int NUM_VEHICLES_PER_LANE = 3;
const int NUM_LANES = 4;
const int CAR_SPEED = 2;
const int WaterHeight = 104;
const int WinHeight = 23;

// Vehicle class for managing vehicles
class Vehicle {
public:
    Vehicle(int startX, int startY, int speed, SDL_Texture* vehicleTexture);
    void update();
    void reset();
    void render(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;
    int getX() const;
    int getY() const;
    int getSpeed() const;
    void setSpeed(int newSpeed);
    void setWidth(int newWidth);
    

private:
    SDL_Rect vehicleRect;
    int initialX;
    int initialY;
    int speed;
    SDL_Texture* vehicleTexture;
};

// LogsWater class for managing water lanes and logs
class LogsWater {
public:
    LogsWater(int startY, SDL_Texture* logTexture, SDL_Texture* waterTexture, int speed, int numLogs);
    void update();
    void render(SDL_Renderer* renderer) const;
    int getY() const;
    std::vector<Vehicle> logs;
    int logWidth;

private:
    int yPosition;
    int logSpeed;
    int numLogs;
    
    SDL_Texture* logTexture;
    void initializeLogs();
};



// Game class for managing the game
class Game {
public:
    Game();
    ~Game();
    bool initialize(const char* title, int width, int height);
    void run();
    void handleInput(bool& running);
    void update();
    void render();
    void cleanup();
    bool loadResources();
    void initializeLogWaterLanes();
    void initializeVehicleLanes();
    void updateVehicleLanes();

    // Collision detection
    bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);
    void renderLogs(SDL_Renderer* renderer);
    void renderVehicles(SDL_Renderer* renderer);
    void renderFrog(SDL_Renderer* renderer);
    void handleFrogCollisionWithVehicle();
    void handleFrogDrowning();

    // Rendering methods
    static void renderVehicleLanes(SDL_Renderer* renderer,const std::vector<Vehicle>& vehicleLanes, const char* errorMessage);
    static void renderLogsWaterLanes(SDL_Renderer* renderer, const std::vector<LogsWater>& logsWaterLanes, const char* errorMessage);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* frogTexture;
    SDL_Texture* shortLogTexture;
    SDL_Texture* mediumLogTexture;
    SDL_Texture* bigLogTexture;

    int frogX;
    int frogY;
    bool gameOver = false;
    std::vector<LogsWater> logsWaterLanes;
    std::vector<std::vector<Vehicle>> vehicleLanes;
};
