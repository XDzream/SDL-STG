//
// Created by zream on 2025/12/2.
//

#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#endif //SPRITE_H
#include <SDL3/SDL.h>
#include "Renderer.h"

class Sprite {
public:
     // 构造和析构
    Sprite();
    ~Sprite();
    
    // 纹理管理
    bool LoadFromFile(const std::string& filePath, Renderer& renderer);
    void Free();
    
    // 渲染功能
    void Render(Renderer&renderer,int x, int y, int width = 0, int height = 0) const;
    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    
    // 状态查询
    bool IsLoaded() const;
    int GetWidth() const;
    int GetHeight() const;
    
private:
    SDL_Texture* texture;
    int width, height;
    bool isLoaded;



};