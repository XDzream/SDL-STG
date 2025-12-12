//
// Created by zream on 2025/12/11.
//

// BulletFactory.h
#ifndef BULLETFACTORY_H
#define BULLETFACTORY_H

#include "BulletConfig.h"
#include "../entity/BulletBase.h"
#include "../graphics/Sprite.h"
#include <memory>
#include <map>
#include <string>
#include <vector>


class Renderer;
class BulletBase;

class BulletFactory {
public:
    BulletFactory();
    ~BulletFactory();

    // 初始化：加载所有配置和资源
    bool Initialize(const std::string& configDir, Renderer& renderer);

    // 创建子弹：只设置外观和碰撞
    std::unique_ptr<BulletBase> CreateBullet(const std::string& bulletType,
                                            BulletOwner owner,
                                            float x, float y);

    // 获取配置信息（供发射者参考）
    const BulletConfig* GetBulletConfig(const std::string& bulletType) const;

    // 检查是否有某种类型的子弹
    bool HasBulletType(const std::string& bulletType) const;

    // 获取所有可用的子弹类型
    std::vector<std::string> GetAvailableBulletTypes() const;

private:
    // 内部资源管理结构
    struct BulletResources {
        std::shared_ptr<BulletConfig> config;  // 配置信息
        std::shared_ptr<Sprite> sprite;        // 共享纹理
    };

    // 加载配置文件
    bool LoadConfigs(const std::string& configDir, Renderer& renderer);
    
    // 加载单个配置文件
    bool LoadConfigFile(const std::string& filePath, Renderer& renderer);

    // 加载并缓存纹理
    std::shared_ptr<Sprite> LoadAndCacheTexture(const std::string& texturePath, Renderer& renderer);

    // 资源存储
    std::map<std::string, BulletResources> bulletResources;
    std::map<std::string, std::shared_ptr<Sprite>> textureCache;

    Renderer* renderer;
    bool initialized;
};

#endif // BULLETFACTORY_H
