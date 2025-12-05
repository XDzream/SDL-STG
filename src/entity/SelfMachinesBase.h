//
// Created by zream on 2025/12/4.
//

#ifndef SELFMACHINESBASE_H
#define SELFMACHINESBASE_H

#include <memory>
#include <algorithm>
#include "EntityBase.h"
#include "../graphics/Sprite.h"
#include "../graphics/Renderer.h"
#include "../input/InputHandler.h"


// 机体状态枚举
enum class PlayerState {
    NORMAL,         // 正常状态
    FOCUS,          // 集中状态（慢速移动）
    INVINCIBLE,     // 无敌状态
    BOMBING,        // 炸弹释放中
    DEAD            // 死亡状态
};

class SelfMachineBase : public EntityBase {
private:
    bool debugMode;

protected:
    // 基础属性
    float speed;                  // 正常移动速度
    float focusSpeed;             // 集中时的移动速度
    PlayerState currentState;     // 当前状态
    
    // 资源管理
    std::unique_ptr<Sprite> sprite;    // 玩家精灵
    
    // 机体特性（为多机体准备）
    int bombCount;               // 炸弹数量
    float power;                 // 火力值 (0.0f - 4.0f)
    int lives;                   // 生命值
    int bombFragments;           // 炸弹碎片
    
    // 状态计时器
    float invincibleTimer;       // 无敌时间计时器
    float bombTimer;            // 炸弹时间计时器
    
    // 窗口边界（用于边界限制）
    int windowWidth;
    int windowHeight;
    
    // 输入处理器引用
    InputHandler* inputHandler;
    Renderer *renderer;

public:
    // 构造和析构
    SelfMachineBase(InputHandler* inputHandler,int windowWidth = 800, int windowHeight = 600);
    virtual ~SelfMachineBase() = default;

    // 重写基类方法
    void Update(float deltaTime) override;
    void Render(Renderer* renderer) override;
    void Initialize() override;
    void OnDestroy() override;
    void OnCollision(EntityBase* other) override;

    // 输入处理
    void HandleInput(float deltaTime);
    void HandleMovement(float deltaTime);
    void HandleShooting();
    void HandleBomb();

    // 状态管理
    void SetState(PlayerState newState);
    PlayerState GetState() const { return currentState; }
    bool IsInvincible() const;
    bool IsInBombMode() const;

    // 资源操作
    void AddPower(float amount);
    void AddBomb();
    void AddLife();
    void AddBombFragment();
    void UseBomb();
    void TakeDamage();
    
    // 访问器
    float GetSpeed() const { return speed; }
    float GetFocusSpeed() const { return focusSpeed; }
    int GetBombCount() const { return bombCount; }
    float GetPower() const { return power; }
    int GetLives() const { return lives; }
    int GetBombFragments() const { return bombFragments; }
    int GetPowerLevel() const { return static_cast<int>(power); }
    
    // 设置器
    void SetSpeed(float speed) { this->speed = speed; }
    void SetFocusSpeed(float focusSpeed) { this->focusSpeed = focusSpeed; }
    void SetBombCount(int count) { this->bombCount = count; }
    void SetPower(float power) { this->power = std::clamp(power, 0.0f, 4.0f); }
    void SetLives(int lives) { this->lives = lives; }
    void SetBombFragments(int fragments) { this->bombFragments = fragments; }

    // 边界限制
    void ClampToScreen();
    
    // 火力等级检查
    bool CanUpgradePower() const { return power < 4.0f; }
    
    // 继承相关的碰撞体设置
    void SetupPlayerCollider();

    //debug
    void SetDebugMode(bool enable) { debugMode = enable; }
    void RenderDebugInfo(Renderer* renderer);

protected:
    // 自机子类可重写方法
    virtual void DoShoot() {}                    // 具体射击逻辑
    virtual void DoBomb() {}                     // 具体炸弹逻辑
    virtual void OnPowerUp() {}                  // 火力提升时
    virtual void OnBombCollected() {}           // 收集炸弹时
    virtual void OnDeath() {}                    // 死亡时

    // 辅助方法
    void UpdateTimers(float deltaTime);
    void UpdateStateEffects(float deltaTime);
    bool ShouldBombResetHitbox() const;          //bomb清屏子弹
};

#endif //SELFMACHINESBASE_H