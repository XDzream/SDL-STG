//
// Created by zream on 2025/12/4.
//

#include "SelfMachinesBase.h"


SelfMachineBase::SelfMachineBase(InputHandler* inputHandler,int windowWidth, int windowHeight)
    : EntityBase(EntityType::PLAYER),
      inputHandler(inputHandler),renderer(renderer) ,windowWidth(windowWidth), windowHeight(windowHeight),
      currentState(PlayerState::NORMAL),
      bombCount(3), power(1.0f), lives(3), bombFragments(0),
      invincibleTimer(0.0f), bombTimer(0.0f) {

    speed = 0.0f;
    focusSpeed = 0.0f;
    
    sprite = std::make_unique<Sprite>();
}



void SelfMachineBase::ClampToScreen() {
    x = std::max(0.0f, std::min(x, (float)windowWidth - width));
    y = std::max(0.0f, std::min(y, (float)windowHeight - height));
}

//基类方法
void SelfMachineBase::Update(float deltaTime) {
    HandleInput(deltaTime);
    UpdateTimers(deltaTime);
    UpdateStateEffects(deltaTime);
}

void SelfMachineBase::Render(Renderer* renderer) {
    if (sprite && sprite->IsLoaded()) {
        sprite->Render(*renderer, (int)x, (int)y);
    }
    
    // 调试模式：显示碰撞体
    if (debugMode) {
        SDL_FRect collider = GetBounds();
        SDL_SetRenderDrawColor(renderer->GetRenderer(), 255, 0, 0, 128);
        SDL_RenderFillRect(renderer->GetRenderer(), &collider);
    }
}

void SelfMachineBase::Initialize() {
    
    // 设置玩家特有的小圆圈碰撞体
    SetupPlayerCollider();
}

void SelfMachineBase::OnDestroy() {
    if (sprite) {
        sprite->Free();
    }
}

void SelfMachineBase::OnCollision(EntityBase* other) {
    // 根据碰撞对象类型处理
    switch (other->GetType()) {
        case EntityType::ENEMY_BULLET:
            TakeDamage();
            break;
        case EntityType::ITEM_POWER:
            AddPower(0.1f);
            break;
        // ... 其他类型
    }
}

//虚函数
void SelfMachineBase::SetupPlayerCollider() {
    // 基类只设置默认值，子类可重写
    double colliderWidth = 4.0f;   // 默认小圆圈
    double colliderHeight = 4.0f;
    double colliderX = 0.0f;       // 默认在左上角
    double colliderY = 0.0f;
    bool useCustomCollider = true;
}

//input
void SelfMachineBase::HandleInput(float deltaTime) {
    HandleMovement(deltaTime);
    HandleShooting();
    HandleBomb();
}

void SelfMachineBase::HandleMovement(float deltaTime) {
    float currentSpeed = (currentState == PlayerState::FOCUS) ? focusSpeed : speed;
    
    if (inputHandler->IsKeyPressed(SDLK_LEFT)) x -= currentSpeed * deltaTime;
    if (inputHandler->IsKeyPressed(SDLK_RIGHT)) x += currentSpeed * deltaTime;
    if (inputHandler->IsKeyPressed(SDLK_UP)) y -= currentSpeed * deltaTime;
    if (inputHandler->IsKeyPressed(SDLK_DOWN)) y += currentSpeed * deltaTime;
    
    ClampToScreen();
}

void SelfMachineBase::HandleShooting() {
    if (inputHandler->IsKeyPressed(SDLK_z)) {  // 东方用Z射击
        DoShoot();
    }
}

void SelfMachineBase::HandleBomb() {
    if (inputHandler->IsKeyPressed(SDLK_x) && bombCount > 0) {
        UseBomb();
    }
}
//状态管理
void SelfMachineBase::SetState(PlayerState newState) {
    currentState = newState;
    
    switch (newState) {
        case PlayerState::INVINCIBLE:
            invincibleTimer = 2.0f;  // 2秒无敌
            break;
        case PlayerState::FOCUS:
            // 可以添加集中时的特效
            break;
    }
}

bool SelfMachineBase::IsInvincible() const {
    return currentState == PlayerState::INVINCIBLE || currentState == PlayerState::BOMBING;
}

bool SelfMachineBase::IsInBombMode() const {
    return currentState == PlayerState::BOMBING;
}
//资源管理
void SelfMachineBase::AddPower(float amount) {
    power = std::min(power + amount, 4.0f);
    OnPowerUp();
}

void SelfMachineBase::AddBomb() {
    bombCount++;
    OnBombCollected();
}

void SelfMachineBase::AddBombFragment() {
    bombFragments++;
    if (bombFragments >= 10) {  // 10个碎片换一个炸弹
        bombFragments -= 10;
        AddBomb();
    }
}

void SelfMachineBase::UseBomb() {
    if (bombCount > 0 && !IsInBombMode()) {
        bombCount--;
        SetState(PlayerState::BOMBING);
        DoBomb();
    }
}

void SelfMachineBase::TakeDamage() {
    if (!IsInvincible()) {
        lives--;
        SetState(PlayerState::INVINCIBLE);
        OnDeath();
        
        if (lives <= 0) {
            SetState(PlayerState::DEAD);
        }
    }
}
//辅助功能
void SelfMachineBase::UpdateTimers(float deltaTime) {
    if (invincibleTimer > 0) {
        invincibleTimer -= deltaTime;
        if (invincibleTimer <= 0) {
            // 无敌时间结束
        }
    }
    
    if (bombTimer > 0) {
        bombTimer -= deltaTime;
        if (bombTimer <= 0) {
            currentState = PlayerState::NORMAL;
        }
    }
}

void SelfMachineBase::UpdateStateEffects(float deltaTime) {
    // 基类不实现具体特效，交给子类
}

bool SelfMachineBase::ShouldBombResetHitbox() const {
    return IsInBombMode();  // 东方机制：炸弹时无敌
}

