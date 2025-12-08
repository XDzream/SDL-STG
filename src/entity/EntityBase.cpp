//
// Created by zream on 2025/12/3.
//

#include "EntityBase.h"

#include <cmath>
#include <bits/stl_algo.h>

EntityBase::EntityBase(EntityType type,float x,float y,float width,float height):type(type),x(x),y(y),width(width),height(height),isActive(true),rotation(0.0f),scale(1.0f){

}

void EntityBase::SetPosition(float x,float y){
    this->x = x;
    this->y = y;
}

void EntityBase::SetSize(float width,float height){
    this->width = width;
    this->height = height;
}

void EntityBase::SetVelocity(float vx,float vy){
    this->velocityX = vx;
    this->velocityY = vy;
}

void EntityBase::Move(float dx,float dy){
    x += dx;
    y += dy;
}

void EntityBase::MoveTo(float x,float y){
    SetPosition(x,y);
}

SDL_FRect EntityBase::GetBounds() const{
    return SDL_FRect{x,y,width,height};
}

bool EntityBase::IsOutOfBounds(int screenWidth,int screenHeight) const{
    return (x + width < 0 || x > screenWidth || y + height < 0 || y > screenHeight);

}

float EntityBase::DistanceTo(const EntityBase& other) const{
    float dx = GetCenterX() - other.GetCenterX();
    float dy = GetCenterY() - other.GetCenterY();
    return std::sqrt(dx * dx + dy * dy);
}

bool EntityBase::IsCollidingWithCircle(const EntityBase& other) const {
    if (!isCircleCollider || !other.isCircleCollider) {
        return false;  // 至少有一个不是圆形
    }
    
    // 计算两个圆心距离
    float center1X = x + colliderX + colliderRadius;
    float center1Y = y + colliderY + colliderRadius;
    float center2X = other.x + other.colliderX + other.colliderRadius;
    float center2Y = other.y + other.colliderY + other.colliderRadius;
    
    float distanceX = center1X - center2X;
    float distanceY = center1Y - center2Y;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    
    // 比较距离与半径之和
    return distance < (colliderRadius + other.colliderRadius);
}


void EntityBase::ClampPosition(float minX, float minY, float maxX, float maxY) {
    x = std::max(minX, std::min(x, maxX - width));
    y = std::max(minY, std::min(y, maxY - height));
}

void EntityBase::UpdateBoundsBasedOnSprite(int spriteWidth, int spriteHeight) {
    width = spriteWidth;
    height = spriteHeight;
}

void EntityBase::SetRectangleCollider(float x, float y, float width, float height) {
    colliderX = x;
    colliderY = y;
    colliderWidth = width;
    colliderHeight = height;
    useCustomCollider = true;
    isCircleCollider = false;  // 标记为矩形
    colliderType = ColliderType::RECTANGLE;
}

void EntityBase::SetCircleCollider(float centerX, float centerY, float radius) {
    colliderRadius = radius;
    colliderX = centerX - radius;  // 转换为矩形边界（内部使用）
    colliderY = centerY - radius;
    colliderWidth = radius * 2;
    colliderHeight = radius * 2;
    useCustomCollider = true;
    isCircleCollider = true;       // 标记为圆形
    colliderType = ColliderType::CIRCLE;
}

void EntityBase::SetCustomColliderEnabled(bool enabled) {
    useCustomCollider = enabled;
    if (!enabled) {
        // 禁用时重置为精灵边界
        colliderX = 0;
        colliderY = 0;
        colliderWidth = width;
        colliderHeight = height;
        isCircleCollider = false;
        colliderType = ColliderType::NONE;
    }
}

void EntityBase::SetColliderOffset(float x, float y) {
    colliderX = x;
    colliderY = y;
}

void EntityBase::SetColliderSize(float width, float height) {
    colliderWidth = std::max(0.0f, width);   // 防止负值
    colliderHeight = std::max(0.0f, height); // 防止负值
    
    if (isCircleCollider) {
        // 如果是圆形，保持正方形
        float size = std::max(width, height);
        colliderWidth = size;
        colliderHeight = size;
        colliderRadius = size / 2.0f;
    }
}

bool EntityBase::IsCollidingWithMixed(const EntityBase& other) const {
    // 矩形与圆形的碰撞检测
    const EntityBase* circle = nullptr;
    const EntityBase* rectangle = nullptr;
    
    // 确定哪个是圆形，哪个是矩形
    if (isCircleCollider && !other.isCircleCollider) {
        circle = this;
        rectangle = &other;
    } else if (!isCircleCollider && other.isCircleCollider) {
        circle = &other;
        rectangle = this;
    } else {
        return false;  // 两个都是矩形或都是圆形，不适用混合检测
    }
    
    // 找到矩形上最近的点
    float rectRight = rectangle->x + rectangle->width;
    float rectBottom = rectangle->y + rectangle->height;
    
    float closestX = std::clamp(circle->GetCenterX(), rectangle->x, rectRight);
    float closestY = std::clamp(circle->GetCenterY(), rectangle->y, rectBottom);
    
    // 计算圆心到最近点的距离
    float distanceX = circle->GetCenterX() - closestX;
    float distanceY = circle->GetCenterY() - closestY;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    
    return distance < circle->colliderRadius;
}

void EntityBase::RenderCollider(Renderer* renderer, SDL_Color color) const {
    if (!useCustomCollider) {
        // 渲染精灵边界
        SDL_FRect bounds = GetBounds();
        SDL_SetRenderDrawColor(renderer->GetRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer->GetRenderer(), &bounds);
    } else if (isCircleCollider) {
        // 渲染圆形碰撞体（简化为小方块，SDL3支持圆形绘制更好）
        SDL_FRect circleBounds = GetColliderBounds();
        SDL_SetRenderDrawColor(renderer->GetRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer->GetRenderer(), &circleBounds);
    } else {
        // 渲染矩形碰撞体
        SDL_FRect colliderBounds = GetColliderBounds();
        SDL_SetRenderDrawColor(renderer->GetRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer->GetRenderer(), &colliderBounds);
    }
}


