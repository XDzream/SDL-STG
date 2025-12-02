#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

bool running = true;

int main() {
    // 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口（UTF-8标题）
    printf("你好");
    SDL_Window* window = SDL_CreateWindow("测试窗口", 640, 480, 0);
    // 事件循环
    SDL_Event event;
    
    while(running){                                                                                                                                                                                       
        while (SDL_PollEvent(&event)) {  // 阻塞等待事件
          if (event.type == SDL_EVENT_QUIT) {
            running = false;
          }
        }

    }
    // 清理
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}