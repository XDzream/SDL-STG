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

    // 设置精灵（从共享资源复制或引用）
    if (resources.sprite) {
        // 为每个子弹创建独立的Sprite实例（共享纹理资源）
        auto bulletSprite = std::make_unique<Sprite>();
        if (bulletSprite->LoadFromFile(resources.config->texture, *renderer)) {
            bullet->SetSprite(std::move(bulletSprite));
        }
    }

    // 设置碰撞体
    const BulletConfig* config = resources.config.get();
    if (config->collider.type == "circle") {
        bullet->SetCircleCollider(config->collider.radius);
    } else if (config->collider.type == "rect") {
        bullet->SetRectCollider(config->collider.w, config->collider.h);
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
    
    // 使用BulletConfigParser解析JSON
    if (!BulletConfigParser::LoadFromFile(filePath, config)) {
        std::cerr << "Failed to parse config file: " << filePath << std::endl;
        return false;
    }

    // 检查id是否有效
    if (config.id.empty()) {
        std::cerr << "Config file missing 'id' field: " << filePath << std::endl;
        return false;
    }

    // 加载并缓存纹理（如果尚未加载）
    std::string texturePath = config.texture;
    if (textureCache.find(texturePath) == textureCache.end()) {
        auto sprite = LoadAndCacheTexture(texturePath, renderer);
        if (!sprite) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return false;
        }
    }

    // 存储配置和资源
    BulletResources resources;
    resources.config = std::make_shared<BulletConfig>(std::move(config));
    resources.sprite = textureCache[texturePath];

    bulletResources[resources.config->id] = resources;
    
    std::cout << "Loaded bullet config: " << resources.config->id << std::endl;
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

