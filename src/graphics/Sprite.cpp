//
// Created by zream on 2025/12/2.
//

#include "Sprite.h"

#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <cstdio>

Sprite::Sprite() : texture(nullptr), width(0), height(0) , isLoaded(false){

}

Sprite::~Sprite() {
    Free();
}

bool Sprite::LoadFromFile(const std::string& filePath, Renderer& renderer) {
    // 先释放旧资源
    Free();
    
    // 直接加载为Texture
    texture = IMG_LoadTexture(renderer.GetRenderer(), filePath.c_str());
    if (!texture) {
        std::cerr << "Unable to load texture from " << filePath << "! Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // SDL3方式：直接访问结构体成员获取尺寸
    width = texture->w;
    height = texture->h;
    isLoaded = true;
    
    return true;
}

void Sprite::Free() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    width = height = 0;
    isLoaded = false;
}

void Sprite::Render(Renderer& renderer, int x, int y, int renderWidth, int renderHeight) const {
    if (!isLoaded || !texture) {
        return;
    }
    
    // 使用原始尺寸如果指定为0
    int finalWidth = (renderWidth == 0) ? width : renderWidth;
    int finalHeight = (renderHeight == 0) ? height : renderHeight;
    
    // 转换为SDL_FRect
    SDL_FRect destRect = { 
        (float)x, 
        (float)y, 
        (float)finalWidth, 
        (float)finalHeight 
    };
    
    // SDL3使用SDL_RenderTexture和SDL_FRect
    SDL_RenderTexture(renderer.GetRenderer(), texture, nullptr, &destRect);
}

void Sprite::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!texture) return;
    
    // 设置颜色调制
    SDL_SetTextureColorMod(texture, r, g, b);
    
    // 设置透明度调制
    SDL_SetTextureAlphaMod(texture, a);
}

bool Sprite::IsLoaded() const {
    return isLoaded;
}

int Sprite::GetWidth() const {
    return width;
}

int Sprite::GetHeight() const {
    return height;
}

