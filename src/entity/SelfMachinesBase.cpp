//
// Created by zream on 2025/12/4.
//

#include "SelfMachinesBase.h"


SelfMachineBase::SelfMachineBase(InputHandler* inputHandler,int windowWidth, int windowHeight)
    : EntityBase(EntityType::PLAYER),
      inputHandler(inputHandler),renderer(nullptr) ,windowWidth(windowWidth), windowHeight(windowHeight),
      currentState(PlayerState::NORMAL),
      speed(2.0f), focusSpeed(1.0f),  // ✅ 合理的默认速度
      bombCount(3), power(1.0f), lives(3), bombFragments(0),
      invincibleTimer(0.0f), bombTimer(0.0f),
      visualHitPointRadius(2.0f), showHitPoint(false), debugMode(false) { // ✅ 初始化所有成员
    
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
    
    // 渲染集中时的判定点（非调试模式也显示）
    if (showHitPoint) {
        RenderHitPoint(renderer);
    }
    
    // 调试模式：显示碰撞体
    if (debugMode) {
        SDL_Color colliderColor = {255, 0, 0, 128};  // 红色
        if (IsInvincible()) {
            static int flashCounter = 0;
            flashCounter++;
            if (flashCounter % 10 < 5) {
                colliderColor = {255, 255, 0, 128};  // 无敌时黄色闪烁
            }
        }
        RenderCollider(renderer, colliderColor);  // 使用正确的碰撞体渲染
    }
}


void SelfMachineBase::Initialize(Renderer* renderer) {
    this->renderer = renderer;
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
    if (inputHandler->IsKeyPressed(SDLK_Z)) {  // 东方用Z射击
        DoShoot();
    }
}

void SelfMachineBase::HandleBomb() {
    if (inputHandler->IsKeyPressed(SDLK_X) && bombCount > 0) {
        UseBomb();
    }
}
//状态管理
void SelfMachineBase::SetState(PlayerState newState) {
    PlayerState oldState = currentState;
    currentState = newState;
    
    switch (newState) {
        case PlayerState::INVINCIBLE:
            invincibleTimer = 2.0f;
            break;
        case PlayerState::FOCUS:
            if (oldState != PlayerState::FOCUS) {
                UpdateHitPointVisibility();  // 进入集中时更新显示
            }
            break;
        case PlayerState::NORMAL:
            if (oldState == PlayerState::FOCUS) {
                UpdateHitPointVisibility();  // 离开集中时更新显示
            }
            break;
        case PlayerState::BOMBING:
            bombTimer = 3.0f;
            UpdateHitPointVisibility();  // 添加显示更新
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

void SelfMachineBase::AddLife() {
    lives++;
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

//碰撞新设函数
// 在RenderHitPoint()中，视觉判定点应该更小且更明显
void SelfMachineBase::RenderHitPoint(Renderer* renderer) {
    if (!showHitPoint) return;
    
    float centerX = x + width / 2.0f;
    float centerY = y + height / 2.0f;
    
    // 绘制判定点（小绿圈）- 这是视觉辅助
    SDL_SetRenderDrawColor(renderer->GetRenderer(), 0, 255, 0, 255);  // 不透明绿色
    SDL_FRect hitPoint = {
        centerX - visualHitPointRadius,
        centerY - visualHitPointRadius,
        visualHitPointRadius * 2,
        visualHitPointRadius * 2
    };
    SDL_RenderFillRect(renderer->GetRenderer(), &hitPoint);
    
    // 绘制白色小点作为中心
    SDL_SetRenderDrawColor(renderer->GetRenderer(), 255, 255, 255, 255);
    SDL_FRect centerDot = {
        centerX - 1, centerY - 1, 2, 2
    };
    SDL_RenderFillRect(renderer->GetRenderer(), &centerDot);
}

void SelfMachineBase::UpdateHitPointVisibility() {
    showHitPoint = (currentState == PlayerState::FOCUS);  // 集中时显示
}


