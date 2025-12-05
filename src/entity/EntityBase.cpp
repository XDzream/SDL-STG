//
// Created by zream on 2025/12/3.
//

#include "EntityBase.h"

#include <cmath>

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

bool EntityBase::IsCollidingWith(const EntityBase& other) const{
    SDL_FRect bounds1 = GetBounds();

    SDL_FRect bounds2 = other.GetBounds();

     return (bounds1.x < bounds2.x + bounds2.w &&
            bounds1.x + bounds1.w > bounds2.x &&
            bounds1.y < bounds2.y + bounds2.h &&
            bounds1.y + bounds1.h > bounds2.y);

}

void EntityBase::ClampPosition(float minX, float minY, float maxX, float maxY) {
    x = std::max(minX, std::min(x, maxX - width));
    y = std::max(minY, std::min(y, maxY - height));
}

void EntityBase::UpdateBoundsBasedOnSprite(int spriteWidth, int spriteHeight) {
    width = spriteWidth;
    height = spriteHeight;
}


