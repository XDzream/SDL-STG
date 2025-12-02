#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <windows.h>
#include <cstdio>
#include <iostream>
#include "graphics/Renderer.h"
#include "graphics/Sprite.h"
#include "input/InputHandler.h"

bool running = true;

int main() {
    // 将工作目录切换到项目根目录
    SetCurrentDirectory("../");
    
    Uint64 last = SDL_GetPerformanceCounter();
    Uint64 now = 0;
    double deltaTime = 0.0;

    
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // 创建窗口（UTF-8标题）
    Renderer gameRenderer;
    InputHandler inputHandler;
    if(!gameRenderer.Initialize("Test window", 800, 600)){
        std::cout << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // 创建测试精灵
    Sprite testSprite;
    
    // 使用相对路径加载图片
    std::string imagePath = "assert/pic.png";
    if (!testSprite.LoadFromFile(imagePath, gameRenderer)) {
        std::cerr << "Failed to load test sprite from " << imagePath << "! Using fallback rectangle." << std::endl;
    } else {
        std::cout << "Sprite loaded successfully: " << testSprite.GetWidth() << "x" << testSprite.GetHeight() << std::endl;
    }
    // 事件循环
    SDL_Event event;
    
    while(running){
     
      // 时间管理
      now = SDL_GetPerformanceCounter();
      deltaTime = (double)((now - last)*1000 / (double)SDL_GetPerformanceFrequency());
      last = now;
      


      while (SDL_PollEvent(&event)) {  // 阻塞等待事件
        if (event.type == SDL_EVENT_QUIT) {
          running = false;
        }
      }

      // 输入处理
      inputHandler.Update();
      
      // 输入处理测试
     if (inputHandler.IsKeyPressed(SDLK_ESCAPE)) {
        running = false;
      }
    
      if (inputHandler.IsKeyPressed(SDLK_SPACE)) {
        std::cout << "Shooting...\n";
      }
      
      // 测试精灵移动1
      static float spriteX = 400.0f, spriteY = 300.0f;
      if (inputHandler.IsKeyPressed(SDLK_LEFT)) {
        spriteX -= 1.0f * deltaTime;
      }
      if (inputHandler.IsKeyPressed(SDLK_RIGHT)) {
        spriteX += 1.0f * deltaTime;
      }
      if (inputHandler.IsKeyPressed(SDLK_UP)) {
        spriteY -= 1.0f * deltaTime;
      }
      if (inputHandler.IsKeyPressed(SDLK_DOWN)) {
        spriteY += 1.0f * deltaTime;
      }
      
      // 边界限制
      const int windowWidth = 800;
      const int windowHeight = 600;
      const int spriteWidth = testSprite.IsLoaded() ? testSprite.GetWidth() : 32;
      const int spriteHeight = testSprite.IsLoaded() ? testSprite.GetHeight() : 32;
      
      // 限制X坐标
      if (spriteX < 0) spriteX = 0;
      if (spriteX > windowWidth - spriteWidth) spriteX = windowWidth - spriteWidth;
      
      // 限制Y坐标
      if (spriteY < 0) spriteY = 0;
      if (spriteY > windowHeight - spriteHeight) spriteY = windowHeight - spriteHeight;
     
      //渲染
      gameRenderer.SetDrawColor(255, 255, 255, 1);  // 黑色背景
      gameRenderer.Clear();
      
      // 渲染测试精灵1
      if (testSprite.IsLoaded()) {
          testSprite.Render(gameRenderer, (int)spriteX, (int)spriteY);  // 使用移动位置渲染精灵
      } else {
          // 降级渲染：显示一个红色方块作为替代
          SDL_FRect rect = {(float)spriteX, (float)spriteY, 32.0f, 32.0f};
          SDL_SetRenderDrawColor(gameRenderer.GetRenderer(), 255, 0, 0, 255);
          SDL_RenderFillRect(gameRenderer.GetRenderer(), &rect);
      }
      
      gameRenderer.Present();


      //帧率控制
      Uint32 frame_start = SDL_GetTicks();
      const int FPS = 60;
      const int frame_length = 1000/FPS;
      
      Uint32 frame_time = SDL_GetTicks() - frame_start;
      if(frame_time < frame_length){
        SDL_Delay(frame_length - frame_time);
      }




    }
    // 清理
    gameRenderer.Cleanup();
    SDL_Quit();
    return 0;
}