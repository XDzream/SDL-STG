//
// Created by zream on 2025/12/11.
//

// BulletFactory.cpp
#include "BulletFactory.h"
#include "BulletConfigParser.h"
#include "../entity/BulletBase.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

BulletFactory::BulletFactory() : renderer(nullptr), initialized(false) {}

BulletFactory::~BulletFactory() = default;

bool BulletFactory::Initialize(const std::string& configDir, Renderer& gameRenderer) {
    if (initialized) {
        std::cerr << "BulletFactory already initialized" << std::endl;
        return false;
    }

    renderer = &gameRenderer;

    // 加载所有配置文件
    if (!LoadConfigs(configDir, *renderer)) {
        std::cerr << "Failed to load bullet configs from: " << configDir << std::endl;
        return false;
    }

    initialized = true;
    std::cout << "BulletFactory initialized successfully" << std::endl;
    return true;
}

std::unique_ptr<BulletBase> BulletFactory::CreateBullet(const std::string& bulletType,
                                                      BulletOwner owner,
                                                      float x, float y) {
    if (!initialized) {
        std::cerr << "BulletFactory not initialized" << std::endl;
        return nullptr;
    }

    auto it = bulletResources.find(bulletType);
    if (it == bulletResources.end()) {
        std::cerr << "Bullet type not found: " << bulletType << std::endl;
        return nullptr;
    }

    const BulletResources& resources = it->second;

    // 创建子弹实例
    auto bullet = std::make_unique<BulletBase>(owner, x, y);
    
    // 使用 InitializeFromConfig 方法初始化子弹
    if (!bullet->InitializeFromConfig(resources.config.get(), resources.sprite)) {
        std::cerr << "Failed to initialize bullet from config: " << bulletType << std::endl;
        return nullptr;
    }
    
    // 初始化子弹（设置尺寸等）
    bullet->Initialize(renderer);

    return bullet;
}


const BulletConfig* BulletFactory::GetBulletConfig(const std::string& bulletType) const {
    auto it = bulletResources.find(bulletType);
    return (it != bulletResources.end()) ? it->second.config.get() : nullptr;
}

bool BulletFactory::HasBulletType(const std::string& bulletType) const {
    return bulletResources.find(bulletType) != bulletResources.end();
}

std::vector<std::string> BulletFactory::GetAvailableBulletTypes() const {
    std::vector<std::string> types;
    for (const auto& pair : bulletResources) {
        types.push_back(pair.first);
    }
    return types;
}

bool BulletFactory::InitializeExistingBullet(BulletBase* bullet, const std::string& bulletType) {
    if (!bullet || !initialized) {
        return false;
    }
    
    auto it = bulletResources.find(bulletType);
    if (it == bulletResources.end()) {
        std::cerr << "Bullet type not found: " << bulletType << std::endl;
        return false;
    }
    
    const BulletResources& resources = it->second;
    
    // 使用 InitializeFromConfig 方法初始化现有子弹
    if (!bullet->InitializeFromConfig(resources.config.get(), resources.sprite)) {
        std::cerr << "Failed to initialize bullet from config: " << bulletType << std::endl;
        return false;
    }
    
    // 初始化子弹（设置尺寸等）
    bullet->Initialize(renderer);
    
    return true;
}

bool BulletFactory::LoadConfigs(const std::string& configDir, Renderer& renderer) {
    // 确保目录存在
    if (!std::filesystem::exists(configDir)) {
        std::cerr << "Config directory does not exist: " << configDir << std::endl;
        return false;
    }

    // 查找所有JSON文件
    for (const auto& file : std::filesystem::directory_iterator(configDir)) {
        if (file.path().extension() == ".json") {
            if (!LoadConfigFile(file.path().string(), renderer)) {
                std::cerr << "Failed to load config file: " << file.path() << std::endl;
                return false;
            }
        }
    }

    return !bulletResources.empty();
}

bool BulletFactory::LoadConfigFile(const std::string& filePath, Renderer& renderer) {
    BulletConfig config;
    
    if (!BulletConfigParser::LoadFromFile(filePath, config)) {
        return false;
    }

    if (config.id.empty()) {
        return false;
    }

    // 直接调用，让LoadAndCacheTexture处理所有缓存逻辑
    auto sprite = LoadAndCacheTexture(config.texture, renderer);
    if (!sprite) {
        return false;
    }

    BulletResources resources;
    resources.config = std::make_shared<BulletConfig>(std::move(config));
    resources.sprite = sprite;  // 直接使用返回值

    bulletResources[resources.config->id] = resources;
    return true;
}


std::shared_ptr<Sprite> BulletFactory::LoadAndCacheTexture(const std::string& texturePath, Renderer& renderer) {
    // 检查缓存
    auto it = textureCache.find(texturePath);
    if (it != textureCache.end()) {
        return it->second;
    }

    // 加载新纹理
    auto sprite = std::make_shared<Sprite>();
    if (!sprite->LoadFromFile(texturePath, renderer)) {
        std::cerr << "Failed to load texture from: " << texturePath << std::endl;
        return nullptr;
    }

    // 缓存纹理
    textureCache[texturePath] = sprite;
    return sprite;
}

