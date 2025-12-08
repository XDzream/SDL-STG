//
// Created by zream on 2025/12/8.
//

#ifndef TESTPLAYER_H
#define TESTPLAYER_H

#include <string>
#include "../entity/SelfMachinesBase.h"

class TestPlayer : public SelfMachineBase {
public:
    explicit TestPlayer(InputHandler* input, int windowW = 800, int windowH = 600);
    ~TestPlayer() override = default;

    // 生命周期
    void Initialize(Renderer* renderer) override;

protected:
    // 覆盖自机逻辑
    void DoShoot() override;
    void DoBomb() override;
    void OnPowerUp() override;
    void OnBombCollected() override;
    void OnDeath() override;
    void UpdateStateEffects(float deltaTime) override;
    void SetupPlayerCollider() override;

    // 自机定制接口（可选对外公开）
    void SetSpritePath(const std::string& path);
    void SetShootCooldown(float ms);
    void SetBombDuration(float ms);

private:
    // 资源
    std::string spritePath;

    // 射击
    float shootCooldownMs;
    float shootTimerMs;
    int   shotLevel;

    // 炸弹
    float bombDurationMs;
    float bombCooldownMs;
    float bombTimerMs;

    // 判定/尺寸
    float hitboxRadius;
    float spriteWidthOverride;
    float spriteHeightOverride;

    // 视觉/调试
    bool  enableFlashOnFocus;
    float flashTimerMs;

    // 内部辅助
    void FireStraightPattern();
    void UpdateShootTimer(float deltaTime);
    void StartBomb();
    void StopBomb();
};

#endif // TESTPLAYER_H