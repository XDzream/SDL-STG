#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <cstdio>
#include <iostream>
#include "graphics/Renderer.h"
#include "input/InputHandler.h"

bool running = true;

int main() {
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
     
      //渲染
      gameRenderer.SetDrawColor(0, 0, 0, 255);  // 红色
      gameRenderer.Clear();
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