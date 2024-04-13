#include "all.h"
void rendervehicleLanes(SDL_Renderer *renderer, const std::vector<Vehicle> &vehicleLanes, const char *errorMessage);
SDL_Texture *Game::carTexture = nullptr;
SDL_Texture* LogsWater::logTexture = nullptr;

//**************************************************************************************************************************             logs and water

// Constructor for LogsWater
LogsWater::LogsWater(int startY, SDL_Texture *logTexture, SDL_Texture *waterTexture, int speed, int numLogs)
    : yPosition(startY),  waterTexture(waterTexture), logSpeed(speed), numLogs(numLogs)
{
    initializeLogs();
}

// Method to initialize logs within the water lanes
void LogsWater::initializeLogs()
{
    int startX = SCREEN_WIDTH;
    for (int i = 0; i < numLogs; i++)
    {
        // Initialize Vehicle for each log and add it to the logs vector
        logs.push_back(Vehicle(startX, yPosition, logSpeed, logTexture));
        startX -= PLAYER_WIDTH;
    }
}

// Update method
void LogsWater::update()
{
    for (Vehicle &log : logs)
    {
        log.update();
    }
}

// Render method for LogsWater
void LogsWater::render(SDL_Renderer *renderer) const
{

    std::cout << "Rendering water at position: " << yPosition << std::endl;
    // Render water first
    SDL_Rect waterRect = {100, yPosition, SCREEN_WIDTH, PLAYER_HEIGHT};
    SDL_RenderCopy(renderer, waterTexture, nullptr, &waterRect);

    // Then render logs on top of the water
    for (const Vehicle &log : logs)
    {
        std::cout << "Rendering log at position: " << log.getX() << ", " << log.getY() << std::endl;
        SDL_Rect logRect = {log.getX(), log.getY(), PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_RenderCopy(renderer, logTexture, nullptr, &logRect);
    }
}

// Getter method
int LogsWater::getY() const
{
    return yPosition;
}

//***********************************************************************************************************************        vechiles

// Constructor for Vehicle
Vehicle::Vehicle(int startX, int startY, int speed, SDL_Texture *vehicleTexture)
    : initialX(startX), initialY(startY), speed(speed), cartexture(vehicleTexture)
{
    vehicleRect = {startX, startY, PLAYER_WIDTH, PLAYER_HEIGHT};
}

// Update method for Vehicle
void Vehicle::update()
{
    vehicleRect.x -= speed; // Move the vehicle

    // Check if the vehicle is out of bounds and reset if necessary
    if (vehicleRect.x + vehicleRect.w < 0)
    {
        reset();
    }
}

// Reset method for Vehicle
void Vehicle::reset()
{
    vehicleRect.x = initialX;
}

// Render method for Vehicle
void Vehicle::render(SDL_Renderer *renderer) const
{
    if (renderer == nullptr)
    {
        std::cerr << "Renderer is null in Vehicle::render." << std::endl;
        return;
    }

    if (cartexture == nullptr)
    {
        std::cerr << "Texture is null in Vehicle::render." << std::endl;
        return;
    }

    // Render the vehicle texture onto the screen
    SDL_RenderCopy(renderer, cartexture, nullptr, &vehicleRect);
}

// Getter methods
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

// Setter method
void Vehicle::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

//***********************************************************************************************************************          Main game

// Constructor
Game::Game() : window(nullptr), renderer(nullptr), frogTexture(nullptr),
               frogX(SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2), frogY(SCREEN_HEIGHT - PLAYER_HEIGHT),
               shortLogTexture(nullptr), mediumLogTexture(nullptr), bigLogTexture(nullptr)
{
    srand((unsigned int)time(nullptr));

    initializeLogWaterLanes();
    initializeVehicleLanes();
}

// Destructor
Game::~Game()
{
    cleanup();
}

// Initialization method
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
    bool checker = loadResources();
    if (!checker)
    {
        return false;
    }

    return true;
}

bool loadTexture(SDL_Renderer *renderer, const std::string &filePath, SDL_Texture *&texture)
{
    SDL_Surface *loadedSurface = SDL_LoadBMP(filePath.c_str());
    if (!loadedSurface)
    {
        std::cerr << "Unable to load image from path " << filePath << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!texture)
    {
        std::cerr << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Game::loadResources()
{
    bool result = true;

    // Load background texture
    result &= loadTexture(renderer, "resources/background_.bmp", backgroundTexture);

    // Load frog texture
    result &= loadTexture(renderer, "resources/frog.bmp", frogTexture);

    // Load car texture
    result &= loadTexture(renderer, "resources/bus.bmp", carTexture);

    // Load short log texture
    result &= loadTexture(renderer, "resources/short_Log.bmp", shortLogTexture);

    // Load medium log texture
    result &= loadTexture(renderer, "resources/medium_log.bmp", mediumLogTexture);

    // Load big log texture
    result &= loadTexture(renderer, "resources/big_log.bmp", bigLogTexture);

    return result;
}

// Initialization function for Logs and Water Lanes in Game
void Game::initializeLogWaterLanes()
{
    for (int lane = 0; lane < 3; lane++)
    {
        // Calculate y-position for the current lane
        int laneY = (lane + 2) * PLAYER_HEIGHT;

        // Choose the appropriate log texture for each lane
        SDL_Texture *logTexture = nullptr;
        if (lane == 0)
        {
            logTexture = shortLogTexture;
        }
        else if (lane == 1)
        {
            logTexture = mediumLogTexture;
        }
        else if (lane == 2)
        {
            logTexture = bigLogTexture;
        }
        // Then add it to the logsWaterLanes vector
        logsWaterLanes.push_back(LogsWater(laneY, logTexture, nullptr, CAR_SPEED + lane, 3));
    }
}

// Main game loop
void Game::run()
{
    bool running = true;
    Uint32 frameStart;
    int frameTime;

    while (running)
    {
        frameStart = SDL_GetTicks();

        handleInput(running);
        update();
        render();
        if (gameOver)
        {
            running = false;
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000 / FPS)
        {
            SDL_Delay((1000 / FPS) - frameTime);
        }
    }
}

// Handle player input
void Game::handleInput(bool &running)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                if (frogY > 0)
                {
                    frogY -= PLAYER_HEIGHT;
                }
                break;
            case SDLK_DOWN:
                if (frogY + PLAYER_HEIGHT < SCREEN_HEIGHT)
                {
                    frogY += PLAYER_HEIGHT;
                }
                break;
            case SDLK_LEFT:
                if (frogX > 0)
                {
                    frogX -= PLAYER_WIDTH;
                }
                break;
            case SDLK_RIGHT:
                if (frogX + PLAYER_WIDTH < SCREEN_WIDTH)
                {
                    frogX += PLAYER_WIDTH;
                }
                break;
            }
        
        }
    }
}

// Update the game state
void Game::update()
{
    // Update vehicles
    for (int i = 0; i < NUM_LANES; i++)
    {
        for (Vehicle &vehicle : vehicleLanes[i])
        {
            vehicle.update();
        }
    }

    // Update logs and water
    for (LogsWater &logsWater : logsWaterLanes)
    {
        logsWater.update();
    }

    // Check for collisions
    SDL_Rect frogRect = {frogX, frogY, PLAYER_WIDTH, PLAYER_HEIGHT};

    bool isFrogOnLog = false;
    bool isVehiclePresent = false; // Flag to check if a vehicle is present in the current lane
    for (int i = 0; i < 3; i++)
    {
        // Check for collisions with vehicles
        isVehiclePresent = false; // Reset the flag for each lane
        for (const Vehicle& vehicle : vehicleLanes[i])
        {
            SDL_Rect vehicleRect = vehicle.getRect();
            vehicleRect.h = PLAYER_HEIGHT; // Set the height of the vehicle rect to PLAYER_HEIGHT

            if ( (frogY >= vehicleRect.x+PLAYER_WIDTH/3) && (  frogX <= vehicleRect.y ) && checkCollision(frogRect, vehicleRect) && !isFrogOnLog){
                std::cout << "Game Over! Frog hit a vehicle." << std::endl;
                // Handle game over condition gracefully
                gameOver = true;
                return;
            }
            isVehiclePresent = true; // Set the flag if a vehicle is present in the current lane
        }

        // Check for collisions with logs only if there is no vehicle in the current lane
        if (!isVehiclePresent)
        {
            const LogsWater &logsWater = logsWaterLanes[i];
            if (frogY >= logsWater.getY() && frogY <= logsWater.getY() + PLAYER_HEIGHT )
            {
                isFrogOnLog = true;
                for (const Vehicle &log : logsWater.logs)
                {
                    if (checkCollision(frogRect, log.getRect()))
                    {
                        frogX += log.getX() - frogRect.x; // Adjust frog's x-position
                        break;
                    }
                }
            }
            else
            {
                isFrogOnLog = false;
            }
        }
    }

    // Check for collisions with winning and losing spots
    int waterStartX = (2 * PLAYER_HEIGHT);             // Calculate water start position based on lane index
    int waterEndX = waterStartX + (5 * PLAYER_HEIGHT); // Calculate water end position based on lane index

    if (!isFrogOnLog && frogY < (SCREEN_HEIGHT / 2) && (frogX < waterStartX || frogX > waterEndX))
    {
        std::cout << "Game Over! Frog drowned." << std::endl;
        gameOver = true;
        return;
    }

    if (!isFrogOnLog && frogY < 0 && frogX > waterStartX)
    {
        std::cout << "Congratulations! Frog reached the safe spot." << std::endl;
        // Handle winning condition gracefully
        gameOver = true;
        return;
    }
}

// Function to render logs and water lanes
static void renderLogsWaterLanes(SDL_Renderer *renderer, const std::vector<LogsWater> &logsWaterLanes, const char *errorMessage)
{
    if (renderer == nullptr)
    {
        std::cerr << "Renderer is null in renderLogsWaterLanes." << std::endl;
        std::cerr << errorMessage << std::endl;
        return;
    }

    if (logsWaterLanes.empty())
    {
        std::cerr << "Logs and water lanes are empty in renderLogsWaterLanes." << std::endl;
        std::cerr << errorMessage << std::endl;
        return;
    }

    for (const auto &lane : logsWaterLanes)
    {
        // Log before rendering each log and water lane
        std::cout << "Rendering logs and water at position: " << lane.getY() << std::endl;

        // Render the logs and water lane
        lane.render(renderer);
    }
}

// Initialization method for vehicle lanes
void Game::initializeVehicleLanes()
{
    int numLanes = 3; // Adjust as needed

    vehicleLanes.resize(numLanes); // Resize the lanes vector

    // Initialize each lane
    for (int laneIndex = 0; laneIndex < numLanes; ++laneIndex)
    {
        int laneY = SCREEN_HEIGHT - (laneIndex + 1) * PLAYER_HEIGHT;
        for (int i = 0; i < NUM_VEHICLES_PER_LANE; ++i)
        {
            int startX = SCREEN_WIDTH + i * PLAYER_WIDTH;
            vehicleLanes[laneIndex].emplace_back(startX, laneY, CAR_SPEED + laneIndex, carTexture);
        }
    }
}

void Game::updateVehicleLanes()
{
    for (auto &lane : vehicleLanes)
    {
        for (auto &vehicle : lane)
        {
            vehicle.update();

            // Reset vehicle position if it goes off-screen
            if (vehicle.getX() < -vehicle.getWidth())
            {
                vehicle.reset();
            }
        }
    }
}

void Game::renderVehicleLanes(SDL_Renderer *renderer)
{
    for (const auto &lane : vehicleLanes)
    {
        for (const auto &vehicle : lane)
        {
            SDL_Rect vehicleRect = {vehicle.getX(), vehicle.getY(), vehicle.getWidth(), vehicle.getHeight()};

            // Render vehicle if car texture is valid
            if (carTexture)
            {
                SDL_RenderCopy(renderer, carTexture, nullptr, &vehicleRect);
            }
            else
            {
                std::cerr << "Car texture is null in renderVehicleLanes." << std::endl;
            }
        }
    }
}

void Game::renderLogs()
{
    // Iterate through each LogsWater lane
    for (const auto &logsWater : logsWaterLanes)
    {
        // Iterate through each log in the current lane
        for (const auto &log : logsWater.logs)
        {
            // Calculate the rectangle for rendering the log
            SDL_Rect logRect = {log.getX(), log.getY(), log.getWidth(), log.getHeight()};

            // Render the log texture
            SDL_RenderCopy(renderer, LogsWater::getTexture(), nullptr, &logRect);
        }
    }
}

void Game::render()
{
    // Clear the renderer
    SDL_RenderClear(renderer);

    // Render the background texture
    if (backgroundTexture != nullptr)
    {
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);
    }
    else
    {
        std::cerr << "Background texture is null in Game::render." << std::endl;
    }

    // Render vehicles, logs, and the frog
    renderLogs(renderer);
    renderVehicles(renderer);
    
    renderFrog(renderer);

    // Present the rendered content
    SDL_RenderPresent(renderer);
}

void Game::renderLogs(SDL_Renderer *renderer)
{
    for (const auto &logsWater : logsWaterLanes)
    {
        if (!logsWater.logs.empty())
        {
            std::vector<SDL_Rect> logRects;
            for (const auto &log : logsWater.logs)
            {
                SDL_Rect logRect = log.getRect();
                logRect.y = SCREEN_HEIGHT - PLAYER_HEIGHT * 3; // Set the y-coordinate to the bottom of the blue area
                logRects.emplace_back(logRect);
            }

            SDL_RenderCopy(renderer, logsWater.getTexture(), nullptr, logRects.data());
        }
    }
}

void Game::renderVehicles(SDL_Renderer *renderer)
{
    int lane = 0;
    for (const auto &vehicleLane : vehicleLanes)
    {
        if (!vehicleLane.empty())
        {
            std::vector<SDL_Rect> vehicleRects;
            for (const auto &vehicle : vehicleLane)
            {
                SDL_Rect vehicleRect = vehicle.getRect();
                vehicleRect.y = SCREEN_HEIGHT - PLAYER_HEIGHT * (lane + 2) - 10; // Set the y-coordinate based on the current lane
                vehicleRects.emplace_back(vehicleRect);
            }
            SDL_RenderCopy(renderer, carTexture, nullptr, vehicleRects.data());
        }
        lane++;
    }
}

void Game::renderFrog(SDL_Renderer *renderer)
{
    if (frogTexture)
    {
        SDL_Rect frogRect = {frogX, frogY, PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_RenderCopy(renderer, frogTexture, nullptr, &frogRect);
    }
    else
    {
        std::cerr << "Frog texture is null in rendering frog." << std::endl;
    }
}

void rendervehicleLanes(SDL_Renderer *renderer, const std::vector<Vehicle> &vehicleLanes, const char *errorMessage)
{
    // Check for null renderer
    if (renderer == nullptr)
    {
        std::cerr << "Renderer is null in renderVehicleLanes." << std::endl;
        std::cerr << errorMessage << std::endl;
        return;
    }

    // Check if vehicle lanes are empty
    if (vehicleLanes.empty())
    {
        std::cerr << "Vehicle lanes are empty in renderVehicleLanes." << std::endl;
        std::cerr << errorMessage << std::endl;
        return;
    }

    // Loop through each vehicle in the vehicle lanes
    for (const auto &vehicle : vehicleLanes)
    {
        // Log the vehicle position
        std::cout << "Rendering vehicle at position: (" << vehicle.getX() << ", " << vehicle.getY() << ")" << std::endl;

        // Get the vehicle's SDL_Rect (position and size)
        SDL_Rect vehicleRect = {vehicle.getX(), vehicle.getY(), vehicle.getWidth(), vehicle.getHeight()};

        // Render the vehicle texture if it is not null
        if (Game::carTexture != nullptr)
        {
            SDL_RenderCopy(renderer, Game::carTexture, nullptr, &vehicleRect);
        }
        else
        {
            std::cerr << "Vehicle texture is null in renderVehicleLanes." << std::endl;
            std::cerr << errorMessage << std::endl;
        }
    }
}


void Game::cleanup()
{
    // Destroy log textures
    for (const auto &logsWater : logsWaterLanes)
    {
        if (logsWater.getTexture())
        {
            SDL_DestroyTexture(logsWater.getTexture());
        }
    }

    // Destroy other textures
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
    if (waterTexture)
    {
        SDL_DestroyTexture(waterTexture);
        waterTexture = nullptr;
    }

    // Destroy renderer and window
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

// Collision detection function
bool Game::checkCollision(const SDL_Rect &rect1, const SDL_Rect &rect2)
{
    return SDL_HasIntersection(&rect1, &rect2);
}
