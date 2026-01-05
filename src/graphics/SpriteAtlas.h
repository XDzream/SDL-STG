//
// Created by zream on 2026/1/5.
//

#ifndef SPRITEATLAS_H
#define SPRITEATLAS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>
#include "Sprite.h"
#include "Renderer.h"

struct FrameData {
    std::string name;
    SDL_Rect rect;
    float duration;  // 帧持续时间（秒），用于动画系统
};

class SpriteAtlas {
public:
    SpriteAtlas();
    ~SpriteAtlas();

    // 配置加载
    bool LoadConfig(const std::string& configPath);
    bool LoadConfig(const std::string& configPath, const std::string& texturePath);
    
    // 帧数据访问
    bool HasFrame(const std::string& frameName) const;
    SDL_Rect GetFrameRect(const std::string& frameName) const;
    const FrameData* GetFrameData(const std::string& frameName) const;
    
    // 动画系统接口
    std::vector<std::string> GetAnimationFrames(const std::string& animationName) const;
    float GetFrameDuration(const std::string& frameName) const;
    
    // 批量操作
    const std::unordered_map<std::string, FrameData>& GetAllFrames() const;
    std::vector<std::string> GetAllFrameNames() const;
    
    // 状态查询
    bool IsLoaded() const;
    const std::string& GetTexturePath() const;
    size_t GetFrameCount() const;

private:
    std::unordered_map<std::string, FrameData> frames;  // 帧名到帧数据的映射
    std::unordered_map<std::string, std::vector<std::string>> animations;  // 动画名到帧序列的映射
    std::string texturePath;
    bool isLoaded;
    
    bool ParseConfig(const std::string& configPath);
};



#endif //SPRITEATLAS_H
