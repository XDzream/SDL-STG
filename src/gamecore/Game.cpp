//
// Created by zream on 2025/12/2.
//

#include "Game.h"
#include "../player/TestPlayer.h"

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

    if(!gameRenderer->Initialize(windowTitle, windowWidth, windowHeight)){
        std::cout << "Failed to initialize renderer" << std::endl;
        return false;
    }

    // 创建并初始化玩家
    player = std::make_unique<TestPlayer>(gameInputHandler.get(), windowWidth, windowHeight);
    player->Initialize(gameRenderer.get());

    gameRunning = true;

    return true;
}

void Game::Cleanup(){
    if(gameRenderer){
        gameRenderer->Cleanup();
        gameRenderer.reset();
    }

    if(gameInputHandler){
        gameInputHandler.reset();
    }

    if(player){
        player.reset();
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
    
    // 更新玩家
    if (player) {
        player->Update(static_cast<float>(deltaTime));
    }
}

void Game::Render() {
    // 设置白色背景并清除屏幕 - 从main.cpp移植
    gameRenderer->SetDrawColor(255, 255, 255, 255);
    gameRenderer->Clear();
    
    // 渲染玩家
    if (player) {
        player->Render(gameRenderer.get());
    }
    
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


// 测试精灵逻辑已移除，改为玩家系统






