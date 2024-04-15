#include "all.h"
bool loadTexture(SDL_Renderer *renderer, const std::string &filePath, SDL_Texture *&texture);
// Forward declaration of classes
class LogsWater;
class Vehicle;
class Game;
int lives = 100000;
// Global texture pointers
SDL_Texture *carTexture = nullptr;
SDL_Texture *logTexture = nullptr;

SDL_Texture *FAIL_TEXTURE = nullptr;

// LogsWater methods implementation

LogsWater::LogsWater(int startY, SDL_Texture *logTexture, SDL_Texture *waterTexture, int speed, int numLogs)
    : yPosition(startY), logSpeed(speed), numLogs(numLogs), logTexture(logTexture)
{
    initializeLogs();
}

void LogsWater::initializeLogs()
{
    int startX = SCREEN_WIDTH;
    for (int i = 0; i < numLogs; i++)
    {
        // Determine the size of the log
        int logWidth;
        if (0 ==  rand()%3)
        {
            logWidth = PLAYER_WIDTH;
        }
        else if (1 == rand() % 3)
        {
            logWidth = MEDIUM_LOG_LENGTH;
        }
        else
        {
            logWidth = BIG_LOG_LENGTH;
        }

        // Create a vehicle object representing the log with the determined width
        Vehicle log(startX, yPosition, logSpeed, logTexture);
        log.setWidth(logWidth);

        // Add the log to the logs vector
        logs.push_back(log);

        // Move the starting x position for the next log
        startX -= logWidth;
    }
}


void LogsWater::update()
{
    for (Vehicle &log : logs)
    {
        log.update();
    }
}

void LogsWater::render(SDL_Renderer *renderer) const
{
    for (const Vehicle &log : logs)
    {
        SDL_Rect logRect = {log.getX(), log.getY(), log.getRect().w, log.getRect().h};
        if (logTexture)
        {
            SDL_RenderCopy(renderer, logTexture, nullptr, &logRect);
        }
        else
        {
            return;
        }
    }
}

int LogsWater::getY() const
{
    return yPosition;
}

// Vehicle methods implementation

Vehicle::Vehicle(int startX, int startY, int speed, SDL_Texture *vehicleTexture)
    : initialX(startX), initialY(startY), speed(speed)
{
    if (!vehicleTexture)
    {
        this->vehicleTexture = FAIL_TEXTURE;
        return;
    }
    this->vehicleTexture = vehicleTexture;
    vehicleRect = {startX, startY, PLAYER_WIDTH, PLAYER_HEIGHT};
}

void Vehicle::setWidth(int newWidth)
{
    vehicleRect.w = newWidth;
}

void Vehicle::update()
{
    vehicleRect.x -= speed;
    if (vehicleRect.x + vehicleRect.w < 0)
    {
        reset();
    }
}

void Vehicle::reset()
{
    vehicleRect.x = initialX;
}

void Vehicle::render(SDL_Renderer *renderer) const
{
    if (renderer == nullptr)
    {
        return;
    }

    if (vehicleTexture == nullptr)
    {
        return;
    }

    SDL_RenderCopy(renderer, vehicleTexture, nullptr, &vehicleRect);
}

SDL_Rect Vehicle::getRect() const
{
    return vehicleRect;
}

int Vehicle::getX() const
{
    return vehicleRect.x;
}

int Vehicle::getY() const
{
    return vehicleRect.y;
}

int Vehicle::getSpeed() const
{
    return speed;
}

void Vehicle::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

// Game methods implementation

Game::Game()
    : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), frogTexture(nullptr),
      frogX(SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2), frogY(SCREEN_HEIGHT - PLAYER_HEIGHT),
      shortLogTexture(nullptr), mediumLogTexture(nullptr), bigLogTexture(nullptr), gameOver(false)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    initializeLogWaterLanes();
    initializeVehicleLanes();
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return loadResources();
}

bool Game::loadResources()
{
    bool result = true;

    result &= loadTexture(renderer, "resources/background1.png", backgroundTexture);
    result &= loadTexture(renderer, "resources/frog1.png", frogTexture);
    result &= loadTexture(renderer, "resources/car.png", carTexture);
    result &= loadTexture(renderer, "resources/log.png", shortLogTexture);
    result &= loadTexture(renderer, "resources/log.png", mediumLogTexture);
    result &= loadTexture(renderer, "resources/log.png", bigLogTexture);
    result &= loadTexture(renderer, "resources/frog.png", FAIL_TEXTURE);

    if (!result)
    {
        std::cerr << "Failed to load resources!" << std::endl;
        return false;
    }

    return result;
}

bool loadTexture(SDL_Renderer *renderer, const std::string &filePath, SDL_Texture *&texture)
{
    // Load the image as a surface using SDL_image
    SDL_Surface *loadedSurface = IMG_Load(filePath.c_str());
    if (!loadedSurface)
    {
        std::cerr << "Unable to load image from path " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create a texture from the loaded surface
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    // Free the loaded surface as it is no longer needed
    SDL_FreeSurface(loadedSurface);

    // Check for errors in creating the texture
    if (!texture)
    {
        std::cerr << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Game::initializeLogWaterLanes() {
    // Calculate the lane height for the logs water lanes
    int laneHeight = (WaterHeight - WinHeight) / 4;

    // Calculate the starting y-coordinate of the first logs water lane
    int startY = WinHeight + PLAYER_HEIGHT;

    for (int lane = 0; lane < 4; lane++) {
        int laneY = startY + lane * (laneHeight * 2);
        SDL_Texture* logTexture = nullptr;

        // Determine the log texture based on the random lane index
        if (lane == rand() % 3) {
            logTexture = shortLogTexture;
        }
        else if (lane == rand() % 3) {
            logTexture = mediumLogTexture;
        }
        else {
            logTexture = bigLogTexture;
        }

        // Create and add a LogsWater object to the logs water lanes vector
        logsWaterLanes.push_back(LogsWater(laneY, logTexture, nullptr, CAR_SPEED + lane, 3));
    }
}

void Game::initializeVehicleLanes()
{
    // Define the starting y-coordinate of the first vehicle lane
    // Calculate the number of lanes to adjust the starting position
    int numLanes = NUM_LANES;
    vehicleLanes.resize(numLanes);

    // Calculate the starting y-coordinate for the first vehicle lane
    // Starting below the logs water lanes and allowing for a safe zone
    int startY = SCREEN_HEIGHT - PLAYER_HEIGHT -(((float)numLanes)+0.5 )* PLAYER_HEIGHT;
    for (int laneIndex = 0; laneIndex < numLanes; ++laneIndex)
    {
        // Calculate the y-coordinate for the current vehicle lane
        // Each lane is positioned one PLAYER_HEIGHT down from the previous lane
        int laneY = startY + laneIndex * PLAYER_HEIGHT;

        // Add vehicles to the current lane
        for (int i = 0; i < NUM_VEHICLES_PER_LANE; ++i)
        {
            // Calculate the starting x-coordinate for the vehicle
            int startX = SCREEN_WIDTH + i * (PLAYER_WIDTH * 3);

            // Create a vehicle with the starting position and texture
            vehicleLanes[laneIndex].emplace_back(Vehicle(startX, laneY, CAR_SPEED + laneIndex, carTexture));
        }
    }
}



void Game::run()
{
    initializeVehicleLanes();
    initializeLogWaterLanes();
    bool running = true;
    while (!gameOver)
    {
        handleInput(running);
        update();
        render();
        SDL_Delay(1000 / FPS);
    }
}

void Game::handleInput(bool &running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                // Move the frog up if it is not at the top of the screen
                if (frogY - PLAYER_HEIGHT >= 0)
                {
                    frogY -= PLAYER_HEIGHT;
                }
                break;
            case SDLK_DOWN:
                // Move the frog down if it is not at the bottom of the screen
                if (frogY + PLAYER_HEIGHT < SCREEN_HEIGHT)
                {
                    frogY += PLAYER_HEIGHT;
                }
                break;
            case SDLK_LEFT:
                // Move the frog left if it is not at the left edge of the screen
                if (frogX - PLAYER_WIDTH >= 0)
                {
                    frogX -= PLAYER_WIDTH;
                }
                break;
            case SDLK_RIGHT:
                // Move the frog right if it is not at the right edge of the screen
                if (frogX + PLAYER_WIDTH < SCREEN_WIDTH)
                {
                    frogX += PLAYER_WIDTH;
                }
                break;
            }

        }
    }
}

void Game::update() {
    // Early exit if the game is already over
    if (gameOver) {
        return;
    }

    // Update logs water lanes
    for (LogsWater& logsWater : logsWaterLanes) {
        logsWater.update();
    }

    // Update vehicle lanes
    updateVehicleLanes();
    // Update logs and water lanes
    for (LogsWater& logsWater : logsWaterLanes)
    {
        logsWater.update();
    }

    // Define the frog's rectangle based on its current position and size
    SDL_Rect frogRect = { frogX, frogY, PLAYER_WIDTH, PLAYER_HEIGHT };

    bool isFrogOnLog = false;

    // Check for collisions between the frog and logs in the logs water lanes
    if (frogY < SCREEN_HEIGHT / 2)
    {
        for (LogsWater& logsWater : logsWaterLanes)
        {
            // Check if the frog's y-position is within the current logs water lane
            if (frogY >= logsWater.getY() && frogY < logsWater.getY() + PLAYER_HEIGHT)
            {
                // Iterate through each log in the current lane
                for (Vehicle& log : logsWater.logs)
                {
                    // Check for collision between the frog and the log
                    if (checkCollision(frogRect, log.getRect()))
                    {
                        // The frog is on top of the log
                        isFrogOnLog = true;
                        // Update the frog's x-position based on the log's speed
                        frogX -= log.getSpeed();
                        break; // Exit the loop as the frog is on a log
                    }
                }

                // If the frog is on a log, exit the outer loop
                if (isFrogOnLog)
                {
                    break;
                }
            }
        }

        // If the frog is not on a log in any water lane, the frog has fallen into the water and drowned
        if (!isFrogOnLog)
        {
            std::cout << "Game Over! Frog drowned." << std::endl;
            handleFrogDrowning();
            return;
        }
    }

    // Check for collisions between the frog and vehicles
    for (const auto& lane : vehicleLanes)
    {
        for (const auto& vehicle : lane)
        {
            // Check for collision between the frog and the vehicle
            if (checkCollision(frogRect, vehicle.getRect()))
            {
                // Handle collision between the frog and a vehicle
                std::cout << "Game Over! Frog hit a vehicle." << std::endl;
                handleFrogCollisionWithVehicle();
                return;
            }
        }
    }

}

void Game::handleFrogCollisionWithVehicle() {
    // Handle frog collision with vehicle
    frogX = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    frogY = SCREEN_HEIGHT - PLAYER_HEIGHT;
    lives--;
    if (lives == 0) {
        gameOver = true;
    }
}

void Game::handleFrogDrowning() {
    // Handle frog drowning
    frogX = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    frogY = SCREEN_HEIGHT - PLAYER_HEIGHT;
    lives--;
    if (lives == 0) {
        gameOver = true;
    }
}


void Game::updateVehicleLanes()
{
    for (auto &lane : vehicleLanes)
    {
        for (auto &vehicle : lane)
        {
            vehicle.update();
        }
    }
}

bool Game::checkCollision(const SDL_Rect &rect1, const SDL_Rect &rect2)
{
    return SDL_HasIntersection(&rect1, &rect2);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render background texture
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

    // Render water lanes and logs
    renderLogsWaterLanes(renderer, logsWaterLanes, "Logs water lanes render error!");
    std::vector<Vehicle> flattenedVehicles;
    for (const std::vector<Vehicle> &lane : vehicleLanes)
    {
        flattenedVehicles.insert(flattenedVehicles.end(), lane.begin(), lane.end());
    }
    // Render vehicle lanes
    renderVehicleLanes(renderer, flattenedVehicles, "Vehicle lanes render error!");

    // Render frog
    renderFrog(renderer);

    SDL_RenderPresent(renderer);
}

void Game::renderLogsWaterLanes(SDL_Renderer *renderer, const std::vector<LogsWater> &logsWaterLanes, const char *errorMessage)
{
    for (const LogsWater &logsWater : logsWaterLanes)
    {
        logsWater.render(renderer);
    }
}

void Game::renderVehicleLanes(SDL_Renderer *renderer, const std::vector<Vehicle> &vehicleLanes, const char *errorMessage)
{

    for (const auto &lane : vehicleLanes)
    {
        lane.render(renderer);
    }
}

void Game::renderFrog(SDL_Renderer *renderer)
{
    SDL_Rect frogRect = {frogX, frogY, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_RenderCopy(renderer, frogTexture, nullptr, &frogRect);
}

void Game::cleanup()
{
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (frogTexture)
    {
        SDL_DestroyTexture(frogTexture);
        frogTexture = nullptr;
    }

    if (carTexture)
    {
        SDL_DestroyTexture(carTexture);
        carTexture = nullptr;
    }

    if (shortLogTexture)
    {
        SDL_DestroyTexture(shortLogTexture);
        shortLogTexture = nullptr;
    }

    if (mediumLogTexture)
    {
        SDL_DestroyTexture(mediumLogTexture);
        mediumLogTexture = nullptr;
    }

    if (bigLogTexture)
    {
        SDL_DestroyTexture(bigLogTexture);
        bigLogTexture = nullptr;
    }

    SDL_Quit();
}