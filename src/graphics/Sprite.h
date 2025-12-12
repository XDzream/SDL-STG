//
// Created by zream on 2025/12/2.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <string>
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
    // 默认整图渲染；如需精灵表裁剪，传入 src 矩形（SDL_FRect）
    void Render(Renderer &renderer, int x, int y, int width = 0, int height = 0, const SDL_FRect *src = nullptr) const;
    // 便捷重载：接受 SDL_Rect，内部转换为 SDL_FRect
    void Render(Renderer &renderer, int x, int y, int width, int height, const SDL_Rect *src) const;
    // 按帧索引渲染精灵表（frameWidth/frameHeight 为帧尺寸；columns 为每行帧数，0 表示按纹理宽自动计算）
    void RenderFrame(Renderer& renderer, int frameIndex, int frameWidth, int frameHeight,
                     int x, int y, float scale = 1.0f, int columns = 0) const;
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

#endif //SPRITE_H