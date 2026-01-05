//
// Created by zream on 2026/1/4.
//

#include "Animator.h"
#include <iostream>

Animator::Animator() 
    : currentAnimation(nullptr),
      lastFrameIndex(-1),
      defaultLoop(true),
      globalSpeed(1.0f) {
}

void Animator::AddAnimation(const std::string& stateName, const std::vector<std::string>& frameNames, 
                            float frameDuration, bool loop) {
    auto animation = std::make_unique<Animation>();
    animation->SetFrames(frameNames);
    animation->SetUniformDuration(frameDuration);
    animation->SetLoop(loop);
    animation->SetSpeed(globalSpeed);
    
    animations[stateName] = std::move(animation);
}

void Animator::AddAnimation(const std::string& stateName, const std::vector<std::string>& frameNames, 
                            const std::vector<float>& frameDurations, bool loop) {
    auto animation = std::make_unique<Animation>();
    animation->SetFrames(frameNames);
    animation->SetFrameDurations(frameDurations);
    animation->SetLoop(loop);
    animation->SetSpeed(globalSpeed);
    
    animations[stateName] = std::move(animation);
}

void Animator::Play(const std::string& stateName, bool forceRestart) {
    if (!HasAnimation(stateName)) {
        std::cerr << "Animator: 动画状态 '" << stateName << "' 不存在" << std::endl;
        return;
    }
    
    SwitchToState(stateName, forceRestart);
}

void Animator::Stop() {
    if (currentAnimation) {
        currentAnimation->Stop();
    }
    currentState.clear();
    currentAnimation = nullptr;
}

void Animator::Pause() {
    if (currentAnimation) {
        currentAnimation->Pause();
    }
}

void Animator::Resume() {
    if (currentAnimation) {
        currentAnimation->Resume();
    }
}

void Animator::Update(float deltaTime) {
    if (!currentAnimation) {
        return;
    }
    
    // 更新当前动画
    currentAnimation->Update(deltaTime);
    
    // 检查并触发回调
    CheckAndTriggerCallbacks();
}

const std::string& Animator::GetCurrentFrameName() const {
    static const std::string emptyString = "";
    
    if (currentAnimation) {
        return currentAnimation->GetCurrentFrameName();
    }
    
    return emptyString;
}

int Animator::GetCurrentFrameIndex() const {
    if (currentAnimation) {
        return currentAnimation->GetCurrentFrameIndex();
    }
    return -1;
}

bool Animator::IsPlaying() const {
    return currentAnimation && currentAnimation->IsPlaying();
}

bool Animator::IsPaused() const {
    return currentAnimation && currentAnimation->IsPaused();
}

const std::string& Animator::GetCurrentState() const {
    return currentState;
}

bool Animator::HasAnimation(const std::string& stateName) const {
    return animations.find(stateName) != animations.end();
}

float Animator::GetProgress() const {
    if (currentAnimation) {
        return currentAnimation->GetProgress();
    }
    return 0.0f;
}

void Animator::SetOnAnimationStart(AnimationCallback callback) {
    onAnimationStart = callback;
}

void Animator::SetOnAnimationComplete(AnimationCallback callback) {
    onAnimationComplete = callback;
}

void Animator::SetOnFrameChange(AnimationCallback callback) {
    onFrameChange = callback;
}

void Animator::SetDefaultLoop(bool loop) {
    this->defaultLoop = loop;
}

void Animator::SetSpeed(float speed) {
    this->globalSpeed = speed;
    
    // 更新所有已注册动画的速度
    for (auto& pair : animations) {
        pair.second->SetSpeed(speed);
    }
}

void Animator::SwitchToState(const std::string& stateName, bool forceRestart) {
    // 如果是同一个状态且不强制重启，则不做处理
    if (currentState == stateName && !forceRestart) {
        return;
    }
    
    // 保存上一个状态
    previousState = currentState;
    currentState = stateName;
    
    // 获取新动画
    auto it = animations.find(stateName);
    if (it != animations.end()) {
        currentAnimation = it->second.get();
        currentAnimation->Play();
        lastFrameIndex = -1;  // 重置帧索引
        
        // 触发动画开始回调
        if (onAnimationStart) {
            onAnimationStart(currentState);
        }
    }
}

void Animator::CheckAndTriggerCallbacks() {
    if (!currentAnimation) {
        return;
    }
    
    // 检查帧变化
    int currentFrameIndex = currentAnimation->GetCurrentFrameIndex();
    if (currentFrameIndex != lastFrameIndex) {
        lastFrameIndex = currentFrameIndex;
        
        if (onFrameChange) {
            onFrameChange(currentState);
        }
    }
    
    // 检查动画完成（仅非循环动画）
    if (currentAnimation->IsFinished() && onAnimationComplete) {
        onAnimationComplete(currentState);
    }
}