//
// Created by zream on 2025/12/2.
//

#include "Game.h"

Game::Game() {
  gameRunning = true;
  lastFrameTime = 0;
  currentFrameTime = 0;
  deltaTime = 0.0;
}

Game::~Game() {
   Cleanup();
}

int Game::Run() {
  if (!Initialize()) {
    return -1;
  }

  while (gameRunning) {
    currentFrameTime = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentFrameTime - lastFrameTime) * 1000 / (double)SDL_GetPerformanceFrequency());
    lastFrameTime = currentFrameTime;

    HandleEvents();
    Update();
    Render();
    FrameRateControl();
  }

  Cleanup();
  return 0;
}

bool Game::Initialize(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SetCurrentDirectory("../");

    gameRenderer = std::make_unique<Renderer>();
    gameInputHandler = std::make_unique<InputHandler>();
    gameSprite = std::make_unique<Sprite>();

    if(!gameRenderer->Initialize(windowTitle, windowWidth, windowHeight)){
        std::cout << "Failed to initialize renderer" << std::endl;
        return false;
    }

    std::string imagePath = "assert/pic.png";

    if(!gameSprite->LoadFromFile(imagePath, *gameRenderer)){
        std::cerr << "Failed to load test sprite from " << imagePath << "! Using fallback rectangle." << std::endl;

    }else{

        std::cout << "Sprite loaded successfully: " << gameSprite->GetWidth() << "x" << gameSprite->GetHeight() << std::endl;

    }

    gameRunning = true;

    return true;
}

void Game::Cleanup(){
    if(gameSprite){
        gameSprite->Free();
        gameSprite.reset();
    }

    if(gameRenderer){
        gameRenderer->Cleanup();
        gameRenderer.reset();
    }

    if(gameInputHandler){
        gameInputHandler.reset();
    }
    
    SDL_Quit();
}

void Game::Update() {
    // 输入处理更新
    gameInputHandler->Update();
    
    // ESC键退出 - 从main.cpp移植
    if (gameInputHandler->IsKeyPressed(SDLK_ESCAPE)) {
        gameRunning = false;
    }
    
    // 测试射击按键
    if (gameInputHandler->IsKeyPressed(SDLK_SPACE)) {
        std::cout << "Shooting...\n";
    }
    
    // 更新测试精灵
    UpdateTestSprite();
}

void Game::Render() {
    // 设置白色背景并清除屏幕 - 从main.cpp移植
    gameRenderer->SetDrawColor(255, 255, 255, 255);
    gameRenderer->Clear();
    
    // 渲染测试精灵
    RenderTestSprite();
    
    // 呈现画面
    gameRenderer->Present();
}

void Game::HandleEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            gameRunning = false;
        }
    }
}

void Game::FrameRateControl() {
    // 从main.cpp移植的帧率控制逻辑
    const int FPS = 60;
    const int frameLength = 1000 / FPS;
    
    Uint32 frameStart = SDL_GetTicks();
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    
    if (frameTime < frameLength) {
        SDL_Delay(frameLength - frameTime);
    }
}


//测试精灵图
void Game::UpdateTestSprite() {
    const float moveSpeed = 1.0f;
    
    if (gameInputHandler->IsKeyPressed(SDLK_LEFT)) {
        testSpriteX -= moveSpeed * deltaTime;
    }
    if (gameInputHandler->IsKeyPressed(SDLK_RIGHT)) {
        testSpriteX += moveSpeed * deltaTime;
    }
    if (gameInputHandler->IsKeyPressed(SDLK_UP)) {
        testSpriteY -= moveSpeed * deltaTime;
    }
    if (gameInputHandler->IsKeyPressed(SDLK_DOWN)) {
        testSpriteY += moveSpeed * deltaTime;
    }
    
    // 边界限制 - 从main.cpp移植
    const int spriteWidth = gameSprite->IsLoaded() ? gameSprite->GetWidth() : 32;
    const int spriteHeight = gameSprite->IsLoaded() ? gameSprite->GetHeight() : 32;
    
    if (testSpriteX < 0) testSpriteX = 0;
    if (testSpriteX > windowWidth - spriteWidth) testSpriteX = windowWidth - spriteWidth;
    if (testSpriteY < 0) testSpriteY = 0;
    if (testSpriteY > windowHeight - spriteHeight) testSpriteY = windowHeight - spriteHeight;
}

void Game::RenderTestSprite() {
    if (gameSprite->IsLoaded()) {
        gameSprite->Render(*gameRenderer, (int)testSpriteX, (int)testSpriteY);
    } else {
        // 降级渲染 - 显示红色方块 - 从main.cpp移植
        SDL_FRect rect = {testSpriteX, testSpriteY, 32.0f, 32.0f};
        SDL_SetRenderDrawColor(gameRenderer->GetRenderer(), 255, 0, 0, 255);
        SDL_RenderFillRect(gameRenderer->GetRenderer(), &rect);
    }
}






