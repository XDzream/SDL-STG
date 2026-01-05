//
// Created by zream on 2026/1/5.
//

#include "SpriteAtlas.h"
#include <fstream>
#include <iostream>
#include "../json.hpp"

using json = nlohmann::json;

SpriteAtlas::SpriteAtlas() : isLoaded(false) {
}

SpriteAtlas::~SpriteAtlas() {
    // 纯数据类，无需清理资源
}

bool SpriteAtlas::LoadConfig(const std::string& configPath) {
    return ParseConfig(configPath);
}

bool SpriteAtlas::LoadConfig(const std::string& configPath, const std::string& texturePath) {
    this->texturePath = texturePath;
    return ParseConfig(configPath);
}

bool SpriteAtlas::ParseConfig(const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        std::cerr << "无法打开配置文件: " << configPath << std::endl;
        return false;
    }
    
    try {
        json config = json::parse(configFile);
        frames.clear();
        animations.clear();
        
        // 获取纹理路径
        if (config.contains("texture")) {
            texturePath = config["texture"];
        }
        
        // 解析帧数据
        if (config.contains("frames") && config["frames"].is_object()) {
            for (auto& [frameName, frameData] : config["frames"].items()) {
                FrameData frame;
                frame.name = frameName;
                
                if (frameData.contains("x") && frameData.contains("y") && 
                    frameData.contains("w") && frameData.contains("h")) {
                    
                    frame.rect.x = frameData["x"];
                    frame.rect.y = frameData["y"];
                    frame.rect.w = frameData["w"];
                    frame.rect.h = frameData["h"];
                    
                    // 可选：帧持续时间
                    if (frameData.contains("duration")) {
                        frame.duration = frameData["duration"];
                    } else {
                        frame.duration = 0.1f;  // 默认100ms
                    }
                    
                    frames[frameName] = frame;
                }
            }
        }
        
        // 解析动画序列
        if (config.contains("animations") && config["animations"].is_object()) {
            for (auto& [animName, animFrames] : config["animations"].items()) {
                if (animFrames.is_array()) {
                    std::vector<std::string> frameNames;
                    for (auto& frameName : animFrames) {
                        frameNames.push_back(frameName);
                    }
                    animations[animName] = frameNames;
                }
            }
        }
        
        isLoaded = !frames.empty();
        return isLoaded;
        
    } catch (const std::exception& e) {
        std::cerr << "配置文件解析错误: " << e.what() << std::endl;
        return false;
    }
}

bool SpriteAtlas::HasFrame(const std::string& frameName) const {
    return frames.find(frameName) != frames.end();
}

SDL_Rect SpriteAtlas::GetFrameRect(const std::string& frameName) const {
    auto it = frames.find(frameName);
    if (it != frames.end()) {
        return it->second.rect;
    }
    
    SDL_Rect emptyRect = {0, 0, 0, 0};
    std::cerr << "警告: 未找到帧 '" << frameName << "'" << std::endl;
    return emptyRect;
}

const FrameData* SpriteAtlas::GetFrameData(const std::string& frameName) const {
    auto it = frames.find(frameName);
    return (it != frames.end()) ? &(it->second) : nullptr;
}

std::vector<std::string> SpriteAtlas::GetAnimationFrames(const std::string& animationName) const {
    auto it = animations.find(animationName);
    return (it != animations.end()) ? it->second : std::vector<std::string>();
}

float SpriteAtlas::GetFrameDuration(const std::string& frameName) const {
    auto it = frames.find(frameName);
    return (it != frames.end()) ? it->second.duration : 0.1f;
}

const std::unordered_map<std::string, FrameData>& SpriteAtlas::GetAllFrames() const {
    return frames;
}

std::vector<std::string> SpriteAtlas::GetAllFrameNames() const {
    std::vector<std::string> names;
    for (const auto& pair : frames) {
        names.push_back(pair.first);
    }
    return names;
}

bool SpriteAtlas::IsLoaded() const {
    return isLoaded;
}

const std::string& SpriteAtlas::GetTexturePath() const {
    return texturePath;
}

size_t SpriteAtlas::GetFrameCount() const {
    return frames.size();
}