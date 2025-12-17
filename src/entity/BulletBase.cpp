 //
// Created by zream on 2025/12/9.
//

#include "BulletBase.h"
#include <cmath>
#include <utility>
#include <iostream>

BulletBase::BulletBase(BulletOwner owner, float x, float y)
    : EntityBase(owner == BulletOwner::PLAYER ? EntityType::PLAYER_BULLET : EntityType::ENEMY_BULLET, 
                 x, y, 0.0f, 0.0f),
      owner(owner),
      damage(1.0f),
      lifeTimeMs(0.0f),
      livedMs(0.0f),
      accelX(0.0f),
      accelY(0.0f),
      config(nullptr),
      currentFrame(0),
      frameTimer(0.0f) {
    // 默认激活
    SetActive(true);
}

void BulletBase::Initialize(Renderer* renderer) {
    (void)renderer; // 基类不强制加载资源，子类可在此加载 sprite
    // 默认碰撞体：小圆形，半径 4 像素
    SetCircleCollider(4.0f);
}

bool BulletBase::InitializeFromConfig(const BulletConfig* bulletConfig, std::shared_ptr<Sprite> sharedSprite) {
    if (!bulletConfig || !sharedSprite) {
        return false;
    }
    
    // 保存配置和资源
    config = bulletConfig;
    sprite = sharedSprite;
    bulletType = bulletConfig->id;  // 使用id字段而不是name
    
    // 设置碰撞体（根据实际BulletConfig结构）
    if (bulletConfig->collider.type == "circle") {
        SetCircleCollider(bulletConfig->collider.radius);
    } else if (bulletConfig->collider.type == "rect") {
        SetRectCollider(bulletConfig->collider.w, bulletConfig->collider.h);
    } else {
        // 默认使用圆形碰撞体，半径为4
        SetCircleCollider(4.0f);
    }
    
    return true;
}

void BulletBase::Update(float deltaTime) {
    if (!isActive) return;

    // 更新生命周期
    UpdateLifeTime(deltaTime);
    
    // 应用移动
    ApplyMovement(deltaTime);
    
    // 更新动画
    UpdateAnimation(deltaTime);
    
    // 调用自定义更新函数（如果设置了的话）
    if (customUpdate) {
        customUpdate(this, deltaTime);
    }
}



void BulletBase::Render(Renderer* renderer) {
    if (!isActive) return;

    if (sprite && sprite->IsLoaded() && config) {
        // 使用配置的帧进行渲染
        if (!config->frames.empty()) {
            const SDL_Rect& frame = config->frames[currentFrame];
            
            // 目标尺寸（考虑缩放）
            int destWidth = static_cast<int>(frame.w * config->renderScale);
            int destHeight = static_cast<int>(frame.h * config->renderScale);
            
            // 目标位置（居中）
            int destX = static_cast<int>(x - destWidth / 2.0f);
            int destY = static_cast<int>(y - destHeight / 2.0f);
            
            // 使用Sprite的Render方法，传入源矩形进行裁切
            sprite->Render(*renderer, destX, destY, destWidth, destHeight, &frame);
        } else {
            // 没有帧信息，使用默认渲染
            sprite->Render(*renderer, static_cast<int>(x), static_cast<int>(y));
        }
    } else {
        // 无贴图时用小方块占位
        SDL_FRect rect = { x, y, 8.0f, 8.0f };
        SDL_SetRenderDrawColor(renderer->GetRenderer(), 255, 0, 255, 255);
        SDL_RenderFillRect(renderer->GetRenderer(), &rect);
    }
}

void BulletBase::OnCollision(EntityBase* other) {
    if (!other || !isActive) return;
    
    // 获取子弹归属
    BulletOwner bulletOwner = GetOwner();
    
    // 根据碰撞对象类型和子弹归属处理碰撞
    switch (other->GetType()) {
        case EntityType::PLAYER:
            // 敌人子弹与玩家碰撞
            if (bulletOwner == BulletOwner::ENEMY) {
                // 对玩家造成伤害
                // 调用OnHit回调
                OnHit(other);
                // 标记子弹为非活跃状态
                SetActive(false);
            }
            break;
            
        case EntityType::ENEMY:
            // 玩家子弹与敌人碰撞
            if (bulletOwner == BulletOwner::PLAYER) {
                // 对敌人造成伤害
                // 调用OnHit回调
                OnHit(other);
                // 标记子弹为非活跃状态
                SetActive(false);
            }
            break;
            
        case EntityType::PLAYER_BULLET:
            // 子弹之间的碰撞（通常不处理）
            break;
            
        case EntityType::ENEMY_BULLET:
            // 子弹之间的碰撞（通常不处理）
            break;
            
        // 其他类型处理...
    }
}




void BulletBase::SetVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}

void BulletBase::SetSpeedAngle(float speed, float angleRad) {
    velocityX = speed * std::cos(angleRad);
    velocityY = speed * std::sin(angleRad);
}

void BulletBase::SetAcceleration(float ax, float ay) {
    accelX = ax;
    accelY = ay;
}

void BulletBase::SetActive(bool active) {
    isActive = active;
}

bool BulletBase::IsActive() const {
    return isActive;
}

void BulletBase::SetDamage(float dmg) {
    damage = dmg;
}

float BulletBase::GetDamage() const {
    return damage;
}

BulletOwner BulletBase::GetOwner() const {
    return owner;
}

void BulletBase::SetLifeTime(float ms) {
    lifeTimeMs = std::max(0.0f, ms);
    livedMs = 0.0f;
}

bool BulletBase::IsExpired() const {
    return (!isActive) || (lifeTimeMs > 0.0f && livedMs >= lifeTimeMs);
}

void BulletBase::SetCircleCollider(float radius) {
    EntityBase::SetCircleCollider(radius, radius, radius);
}

void BulletBase::SetRectCollider(float w, float h) {
    EntityBase::SetRectangleCollider(0.0f, 0.0f, w, h);
}



void BulletBase::SetCustomUpdate(std::function<void(BulletBase*, float)> customUpdate) {
    this->customUpdate = std::move(customUpdate);
}

const std::string& BulletBase::GetBulletType() const {
    return bulletType;

}




void BulletBase::UpdateLifeTime(float deltaTime) {
    if (lifeTimeMs <= 0.0f) return;
    livedMs += deltaTime;
    if (livedMs >= lifeTimeMs) {
        OnExpire();
        SetActive(false);
    }
}

void BulletBase::ApplyMovement(float deltaTime) {
    // 简单的加速度积分（deltaTime 以毫秒计）
    velocityX += accelX * deltaTime;
    velocityY += accelY * deltaTime;
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
}

void BulletBase::UpdateAnimation(float deltaTime) {
    if (!config || config->frames.size() <= 1) return;  // 单帧无需动画
    
    frameTimer += deltaTime;
    if (frameTimer >= 100.0f) {  // 每100ms切换一帧
        frameTimer -= 100.0f;
        currentFrame = (currentFrame + 1) % config->frames.size();  // 正确访问frames
    }
}
