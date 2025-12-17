//
// Created by zream on 2025/12/9.
//

#ifndef BULLETBASE_H
#define BULLETBASE_H

#include <memory>
#include <functional>
#include "../graphics/Sprite.h"
#include "../graphics/Renderer.h"
#include "../bullet/BulletConfig.h"
#include "EntityBase.h"

class BulletBase;
// 前向声明
struct BulletConfig;

enum class BulletOwner {
    PLAYER,
    ENEMY
};

// 行为组件基类（前向声明，可以在单独文件中实现）
class BulletBehavior {
public:
    virtual ~BulletBehavior() = default;
    
};

class BulletBase : public EntityBase {
public:
    BulletBase(BulletOwner owner, float x = 0.f, float y = 0.f);
    virtual ~BulletBase() = default;

    // 生命周期
    virtual void Initialize(Renderer* renderer);
    
    // 新增：从配置初始化外观和碰撞体
    bool InitializeFromConfig(const BulletConfig* config, std::shared_ptr<Sprite> sharedSprite);
    
    // 更新和渲染（支持自定义行为）
    virtual void Update(float deltaTime) override;
    virtual void Render(Renderer* renderer) override;
    virtual void OnCollision(EntityBase* other) override;

    // 运动与状态（保留原有接口）
    void SetVelocity(float vx, float vy);
    void SetSpeedAngle(float speed, float angleRad);
    void SetAcceleration(float ax, float ay);
    void SetActive(bool active);
    bool IsActive() const;

    // 伤害与归属
    void SetDamage(float dmg);
    float GetDamage() const;
    void SetOwner(BulletOwner newOwner) { owner = newOwner; }
    BulletOwner GetOwner() const;

    // 寿命与边界（保留原有接口）
    void SetLifeTime(float ms);          // 设定存活时间，0 表示不限制
    bool IsExpired() const;

    // 碰撞体设置（保留原有接口）
    void SetCircleCollider(float radius);
    void SetRectCollider(float w, float h);

    // 新增：行为设置接口
    
    void SetCustomUpdate(std::function<void(BulletBase*, float)> customUpdate){this->customUpdate = std::move(customUpdate);}
    
    // 新增：获取配置信息
    const std::string& GetBulletType() const;

protected:
    // 供子类重写的钩子（保留原有接口）
    virtual void OnExpire() {}           // 到时销毁
    virtual void OnHit(EntityBase* other) {} // 碰撞命中回调

    // 内部辅助（保留原有接口）
    void UpdateLifeTime(float deltaTime);
    void ApplyMovement(float deltaTime);
    
    // 新增：内部辅助方法
    void UpdateAnimation(float deltaTime);

    // 原有成员
    std::shared_ptr<Sprite> sprite;
    BulletOwner owner;
    float damage;
    float lifeTimeMs;    // 初始设定寿命
    float livedMs;       // 已存活时间
    float accelX;
    float accelY;
    
    // 新增成员
    const BulletConfig* config;  // 配置信息
    std::string bulletType;      // 子弹类型名称
    
    // 新增：行为相关成员
    
    std::function<void(BulletBase*, float)> customUpdate;
    
    
    // 新增：动画相关成员
    int currentFrame;
    float frameTimer;
};

#endif // BULLETBASE_H