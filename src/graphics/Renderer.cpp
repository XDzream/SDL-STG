//
// Created by zream on 2025/12/2.
//

#include "Renderer.h"
#include <iostream>

Renderer::Renderer():  window(nullptr),renderer(nullptr),isInitialized(false){

}

Renderer::~Renderer() {
    Cleanup();
}

bool Renderer::Initialize(const char* title, int width, int height) {
    window = SDL_CreateWindow(title, width, height, 0);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        Cleanup();
        return false;
    }
    
    isInitialized = true;
    return true;

}

//渲染操作
void Renderer::Cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    isInitialized = false;
}

void Renderer::Present() {
    if(isInitialized && renderer) {
        if (!SDL_RenderPresent(renderer)) {
            std::cerr << "SDL_RenderPresent failed: " << SDL_GetError() << std::endl;
        }
    }
}

void Renderer::Clear() {
    if (isInitialized && renderer) {
        if (!SDL_RenderClear(renderer)) {
            std::cerr << "SDL_RenderClear failed: " << SDL_GetError() << std::endl;
        }
    }
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (isInitialized && renderer) {
        if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
            std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;
        }
    } else {
        std::cerr << "SetDrawColor failed: renderer not initialized" << std::endl;
    }
}

// 状态查询函数实现
SDL_Window* Renderer::GetWindow() const {
    return window;
}

SDL_Renderer* Renderer::GetRenderer() const {
    return renderer;
}

bool Renderer::IsInitialized() const {
    return isInitialized;
}