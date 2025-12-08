//
// Created by zream on 2025/12/8.
//

#include "TestPlayer.h"
#include <iostream>

TestPlayer::TestPlayer(InputHandler* input, int windowW, int windowH)
    : SelfMachineBase(input, windowW, windowH),
      spritePath("assert/pic.png"),
      shootCooldownMs(200.0f),
      shootTimerMs(0.0f),
      shotLevel(1),
      bombDurationMs(2000.0f),
      bombCooldownMs(0.0f),
      bombTimerMs(0.0f),
      hitboxRadius(3.0f),
      spriteWidthOverride(0.0f),
      spriteHeightOverride(0.0f),
      enableFlashOnFocus(true),
      flashTimerMs(0.0f) {}

void TestPlayer::Initialize(Renderer* renderer) {
    SelfMachineBase::Initialize(renderer);

    // 加载自机贴图
    if (sprite && !spritePath.empty()) {
        if (!sprite->LoadFromFile(spritePath, *renderer)) {
            std::cerr << "TestPlayer: failed to load sprite: " << spritePath << "\n";
        } else {
            // 根据精灵尺寸更新实体大小
            int w = sprite->GetWidth();
            int h = sprite->GetHeight();
            if (spriteWidthOverride > 0) w = static_cast<int>(spriteWidthOverride);
            if (spriteHeightOverride > 0) h = static_cast<int>(spriteHeightOverride);
            UpdateBoundsBasedOnSprite(w, h);
        }
    }

    // 初始位置放在窗口中央
    SetPosition((float)(windowWidth - width) * 0.5f, (float)(windowHeight - height) * 0.75f);

    // 设置碰撞体/判定点
    SetupPlayerCollider();
}

void TestPlayer::DoShoot() {
    if (shootTimerMs > 0.0f) {
        return;  // 冷却中
    }

    FireStraightPattern();
    shootTimerMs = shootCooldownMs;
}

void TestPlayer::DoBomb() {
    if (bombTimerMs > 0.0f) {
        return;  // 正在放炸弹或冷却
    }
    StartBomb();
}

void TestPlayer::OnPowerUp() {
    // 简单调整射击间隔/火力等级
    shotLevel = std::min(5, static_cast<int>(power) + 1);
    shootCooldownMs = std::max(60.0f, 200.0f - shotLevel * 20.0f);
}

void TestPlayer::OnBombCollected() {
    std::cout << "TestPlayer: bomb collected\n";
}

void TestPlayer::OnDeath() {
    std::cout << "TestPlayer: dead, remaining lives=" << lives << "\n";
}

void TestPlayer::UpdateStateEffects(float deltaTime) {
    // 更新射击冷却
    UpdateShootTimer(deltaTime);

    // 炸弹计时
    if (bombTimerMs > 0.0f) {
        bombTimerMs -= deltaTime;
        if (bombTimerMs <= 0.0f) {
            StopBomb();
        }
    }

    // 集中态闪烁（可选）
    if (enableFlashOnFocus && currentState == PlayerState::FOCUS) {
        flashTimerMs += deltaTime;
        if (flashTimerMs > 200.0f) {
            flashTimerMs = 0.0f;
            showHitPoint = !showHitPoint;  // 让判定点闪烁
        }
    } else {
        showHitPoint = (currentState == PlayerState::FOCUS);
        flashTimerMs = 0.0f;
    }
}

void TestPlayer::SetupPlayerCollider() {
    // 使用小圆形判定点
    visualHitPointRadius = hitboxRadius;
    SetCircleCollider(width * 0.5f, height * 0.5f, hitboxRadius);
}

void TestPlayer::SetSpritePath(const std::string& path) {
    spritePath = path;
}

void TestPlayer::SetShootCooldown(float ms) {
    shootCooldownMs = std::max(0.0f, ms);
}

void TestPlayer::SetBombDuration(float ms) {
    bombDurationMs = std::max(0.0f, ms);
}

void TestPlayer::FireStraightPattern() {
    // 目前还没有弹幕系统，先输出日志占位
    std::cout << "TestPlayer: shoot level " << shotLevel << "\n";
}

void TestPlayer::UpdateShootTimer(float deltaTime) {
    if (shootTimerMs > 0.0f) {
        shootTimerMs = std::max(0.0f, shootTimerMs - deltaTime);
    }
}

void TestPlayer::StartBomb() {
    bombTimerMs = bombDurationMs;
    SetState(PlayerState::BOMBING);
    std::cout << "TestPlayer: bomb start\n";
}

void TestPlayer::StopBomb() {
    bombTimerMs = 0.0f;
    SetState(PlayerState::NORMAL);
    std::cout << "TestPlayer: bomb end\n";
}
