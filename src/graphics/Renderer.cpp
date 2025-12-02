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
        SDL_RenderPresent(renderer);
    }
}

void Renderer::Clear() {
    if (isInitialized && renderer) {
        SDL_RenderClear(renderer);
    }
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (isInitialized && renderer) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    } else {
        std::cout << "SetDrawColor failed: not initialized" << std::endl;
    }
}