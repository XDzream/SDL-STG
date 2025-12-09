//
// Created by zream on 2025/12/9.
//

#include "BulletBase.h"
#include <cmath>
#include <utility>
#include <iostream>

BulletBase::BulletBase(BulletOwner owner, float x, float y)
    : EntityBase(EntityType::PLAYER_BULLET, x, y, 0.0f, 0.0f),  // 默认按玩家子弹，可由子类修改 type
      owner(owner),
      damage(1.0f),
      lifeTimeMs(0.0f),
      livedMs(0.0f),
      accelX(0.0f),
      accelY(0.0f) {
    // 默认激活
    SetActive(true);
}

void BulletBase::Initialize(Renderer* renderer) {
    (void)renderer; // 基类不强制加载资源，子类可在此加载 sprite
    // 默认碰撞体：小圆形，半径 4 像素
    SetCircleCollider(4.0f);
}

void BulletBase::Update(float deltaTime) {
    if (!isActive) return;

    // 生命周期更新
    UpdateLifeTime(deltaTime);
    if (!isActive) return;

    // 运动
    ApplyMovement(deltaTime);
}

void BulletBase::Render(Renderer* renderer) {
    if (!isActive) return;

    if (sprite && sprite->IsLoaded()) {
        sprite->Render(*renderer, static_cast<int>(x), static_cast<int>(y));
    } else {
        // 无贴图时用小方块占位
        SDL_FRect rect = { x, y, 8.0f, 8.0f };
        SDL_SetRenderDrawColor(renderer->GetRenderer(), 255, 0, 255, 255);
        SDL_RenderFillRect(renderer->GetRenderer(), &rect);
    }
}

void BulletBase::OnCollision(EntityBase* other) {
    if (!isActive) return;
    OnHit(other);
    SetActive(false);
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

void BulletBase::SetSprite(std::unique_ptr<Sprite> spr) {
    sprite = std::move(spr);
    if (sprite && sprite->IsLoaded()) {
        UpdateBoundsBasedOnSprite(sprite->GetWidth(), sprite->GetHeight());
    }
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
