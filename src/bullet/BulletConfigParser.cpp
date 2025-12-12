//
// Created by zream on 2025/12/11.
//

#include "BulletConfigParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

// 注意：需要下载 nlohmann/json.hpp 单头文件库
// 下载地址：https://github.com/nlohmann/json/releases
// 将 json.hpp 放在项目根目录或 src/ 目录下
#include "../json.hpp"

using json = nlohmann::json;

// 内部辅助函数：从json对象解析配置
static bool ParseJsonToConfig(const json& j, BulletConfig& config) {
    try {
        // 解析基本字段
        if (j.contains("id")) {
            config.id = j["id"].get<std::string>();
        }
        
        if (j.contains("texture")) {
            config.texture = j["texture"].get<std::string>();
        }
        
        // 解析frames数组
        if (j.contains("frames") && j["frames"].is_array()) {
            config.frames.clear();
            for (const auto& frame : j["frames"]) {
                int x = frame.contains("x") ? frame["x"].get<int>() : 0;
                int y = frame.contains("y") ? frame["y"].get<int>() : 0;
                int w = frame.contains("w") ? frame["w"].get<int>() : 0;
                int h = frame.contains("h") ? frame["h"].get<int>() : 0;
                config.frames.push_back({x, y, w, h});
            }
        }
        
        // 解析collider
        if (j.contains("collider") && j["collider"].is_object()) {
            const auto& collider = j["collider"];
            if (collider.contains("type")) {
                config.collider.type = collider["type"].get<std::string>();
            }
            if (collider.contains("radius")) {
                config.collider.radius = collider["radius"].get<float>();
            }
            if (collider.contains("w")) {
                config.collider.w = collider["w"].get<float>();
            }
            if (collider.contains("h")) {
                config.collider.h = collider["h"].get<float>();
            }
        }
        
        // 解析render_scale
        if (j.contains("render_scale")) {
            config.renderScale = j["render_scale"].get<float>();
        }
        
        return true;
    } catch (const json::exception& e) {
        std::cerr << "BulletConfigParser: Error parsing JSON: " << e.what() << std::endl;
        return false;
    }
}

bool BulletConfigParser::LoadFromFile(const std::string& filePath, BulletConfig& config) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "BulletConfigParser: Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "BulletConfigParser: JSON parse error: " << e.what() << std::endl;
        file.close();
        return false;
    }
    
    file.close();
    return ParseJsonToConfig(j, config);
}

bool BulletConfigParser::LoadFromString(const std::string& jsonString, BulletConfig& config) {
    try {
        json j = json::parse(jsonString);
        return ParseJsonToConfig(j, config);
    } catch (const json::parse_error& e) {
        std::cerr << "BulletConfigParser: JSON parse error: " << e.what() << std::endl;
        return false;
    }
}

SDL_Rect BulletConfigParser::ParseFrameRect(int x, int y, int w, int h) {
    return {x, y, w, h};
}

