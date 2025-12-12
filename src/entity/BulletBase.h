//
// Created by zream on 2025/12/9.
//

#ifndef BULLETBASE_H
#define BULLETBASE_H

#include <memory>
#include "../graphics/Sprite.h"
#include "../graphics/Renderer.h"
#include "EntityBase.h"
struct Bulletconfig;

enum class BulletOwner {
    PLAYER,
    ENEMY
};

class BulletBase : public EntityBase {
public:
    BulletBase(BulletOwner owner, float x = 0.f, float y = 0.f);
    virtual ~BulletBase() = default;

    // 生命周期
    virtual void Initialize(Renderer* renderer);
    virtual void Update(float deltaTime) override;
    virtual void Render(Renderer* renderer) override;
    virtual void OnCollision(EntityBase* other) override;

    // 运动与状态
    void SetVelocity(float vx, float vy);
    void SetSpeedAngle(float speed, float angleRad);
    void SetAcceleration(float ax, float ay);
    void SetActive(bool active);
    bool IsActive() const;

    // 伤害与归属
    void SetDamage(float dmg);
    float GetDamage() const;
    BulletOwner GetOwner() const;

    // 寿命与边界
    void SetLifeTime(float ms);          // 设定存活时间，0 表示不限制
    bool IsExpired() const;

    // 碰撞体设置（默认圆形）
    void SetCircleCollider(float radius);
    void SetRectCollider(float w, float h);

    // 资源
    void SetSprite(std::unique_ptr<Sprite> spr);

protected:
    // 供子类重写的钩子
    virtual void OnExpire() {}           // 到时销毁
    virtual void OnHit(EntityBase* other) {} // 碰撞命中回调

    // 内部辅助
    void UpdateLifeTime(float deltaTime);
    void ApplyMovement(float deltaTime);

    std::unique_ptr<Sprite> sprite;
    BulletOwner owner;
    float damage;
    float lifeTimeMs;    // 初始设定寿命
    float livedMs;       // 已存活时间
    float accelX;
    float accelY;
};

#endif // BULLETBASE_H
