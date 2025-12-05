//
// Created by zream on 2025/12/3.
//

#ifndef ENTITYBASE_H
#define ENTITYBASE_H

#include <SDL3/SDL.h>
#include "../graphics/Renderer.h"



// 实体类型枚举
enum class EntityType {
    PLAYER,
    ENEMY,
    PLAYER_BULLET,
    ENEMY_BULLET,
    ITEM_POWER,
    ITEM_BOMB,
    ITEM_LIFE,
    ITEM_POINT
};

class EntityBase {


protected:
//基础属性之类的。
    float x,y;
    float width, height;
    bool isActive;
    EntityType type;
    float rotation;
    float scale;

    float velocityX = 0.0f; 
    float velocityY = 0.0f;

public:
    EntityBase(EntityType type , float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);

    virtual ~EntityBase() =default;


    //纯虚函数 
    virtual void Update(float deltaTime) = 0;

    virtual void Render(Renderer* renderer) = 0;

    // 可选的虚函数 - 子类可以重写
    virtual void Initialize() {}

    virtual void OnDestroy() {}

    virtual void OnCollision(EntityBase* other) {}

    // 基础访问器
    [[nodiscard]] float GetX() const { return x; }
    [[nodiscard]] float GetY() const { return y; }
    [[nodiscard]] float GetWidth() const { return width; }
    [[nodiscard]] float GetHeight() const { return height; }
    [[nodiscard]] bool IsActive() const { return isActive; }
    [[nodiscard]] EntityType GetType() const { return type; }
    [[nodiscard]] float GetRotation() const { return rotation; }
    [[nodiscard]] float GetScale() const { return scale; }
    [[nodiscard]] float GetVelocityX() const { return velocityX; }
    [[nodiscard]] float GetVelocityY() const { return velocityY; }

    // 基础设置器
    void SetPosition(float x, float y);
    void SetX(float x) { this->x = x; }
    void SetY(float y) { this->y = y; }
    void SetSize(float width, float height);
    void SetActive(bool active) { this->isActive = active; }
    void SetType(EntityType type) { this->type = type; }
    void SetRotation(float rotation) { this->rotation = rotation; }
    void SetScale(float scale) { this->scale = scale; }
    void SetVelocity(float vx, float vy);
    void SetVelocityX(float vx) { this->velocityX = vx; }
    void SetVelocityY(float vy) { this->velocityY = vy; }

    // 位置操作
    void Move(float dx, float dy);
    void MoveTo(float x, float y);

    // 边界相关
    [[nodiscard]] SDL_FRect GetBounds() const;
    [[nodiscard]] bool IsOutOfBounds(int screenWidth, int screenHeight) const;
    
    // 获取中心点
    [[nodiscard]] float GetCenterX() const { return x + width / 2.0f; }
    [[nodiscard]] float GetCenterY() const { return y + height / 2.0f; }

    // 简单的距离计算（用于碰撞检测）
    [[nodiscard]] float DistanceTo(const EntityBase& other) const;
    [[nodiscard]] bool IsCollidingWith(const EntityBase& other) const;

    // 子类可用的辅助方法
    void ClampPosition(float minX, float minY, float maxX, float maxY);
    void UpdateBoundsBasedOnSprite(int spriteWidth, int spriteHeight);







};
#endif //ENTITYBASE_H