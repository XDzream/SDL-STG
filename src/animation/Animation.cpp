//
// Created by zream on 2025/12/18.
//

#include "Animation.h"
#include <algorithm>
#include <iostream>
#include <cmath>

Animation::Animation() 
    : currentFrameIndex(0),
      currentFrameTime(0.0f),
      speed(1.0f),
      isPlaying(false),
      isPaused(false),
      isLoop(true),
      isFinished(false) {
}

void Animation::SetFrames(const std::vector<std::string>& frameNames) {
    this->frameNames = frameNames;
    
    // 如果没有设置帧持续时间，使用默认值
    if (frameDurations.empty() && !frameNames.empty()) {
        frameDurations.resize(frameNames.size(), 0.1f);  // 默认每帧100ms
    }
    
    Reset();
}

void Animation::SetFrameDurations(const std::vector<float>& durations) {
    this->frameDurations = durations;
}

void Animation::SetUniformDuration(float duration) {
    frameDurations.clear();
    frameDurations.resize(frameNames.size(), duration);
}

void Animation::SetLoop(bool loop) {
    this->isLoop = loop;
}

void Animation::SetSpeed(float speed) {
    this->speed = std::max(0.0f, speed);  // 防止负速度
}

void Animation::Play() {
    Reset();
    isPlaying = true;
    isPaused = false;
    isFinished = false;
}

void Animation::Resume() {
    if (isPaused) {
        isPaused = false;
        isPlaying = true;
    }
}

void Animation::Pause() {
    if (isPlaying) {
        isPaused = true;
        isPlaying = false;
    }
}

void Animation::Stop() {
    isPlaying = false;
    isPaused = false;
    Reset();
}

void Animation::Reset() {
    currentFrameIndex = 0;
    currentFrameTime = 0.0f;
    isFinished = false;
}

void Animation::Update(float deltaTime) {
    // 如果没有帧或者未在播放状态，直接返回
    if (frameNames.empty() || !isPlaying || isPaused) {
        return;
    }
    
    // 如果已经播放完毕且不循环，直接返回
    if (isFinished && !isLoop) {
        return;
    }
    
    // 应用播放速度
    float scaledDeltaTime = deltaTime * speed;
    currentFrameTime += scaledDeltaTime;
    
    // 获取当前帧的持续时间
    float currentDuration = GetCurrentFrameDuration();
    
    // 检查是否需要推进到下一帧
    while (currentFrameTime >= currentDuration && !frameNames.empty()) {
        currentFrameTime -= currentDuration;
        AdvanceFrame();
        
        // 如果完成了且不循环，退出循环
        if (isFinished && !isLoop) {
            currentFrameTime = 0.0f;
            break;
        }
        
        currentDuration = GetCurrentFrameDuration();
    }
}

void Animation::AdvanceFrame() {
    currentFrameIndex++;
    
    // 检查是否到达末尾
    if (currentFrameIndex >= static_cast<int>(frameNames.size())) {
        if (isLoop) {
            currentFrameIndex = 0;  // 循环回到第一帧
        } else {
            currentFrameIndex = static_cast<int>(frameNames.size()) - 1;  // 停在最后一帧
            isFinished = true;
            isPlaying = false;
        }
    }
}

float Animation::GetCurrentFrameDuration() const {
    if (frameDurations.empty() || currentFrameIndex >= static_cast<int>(frameDurations.size())) {
        return 0.1f;  // 默认100ms
    }
    return frameDurations[currentFrameIndex];
}

bool Animation::IsPlaying() const {
    return isPlaying;
}

bool Animation::IsPaused() const {
    return isPaused;
}

bool Animation::IsFinished() const {
    return isFinished;
}

int Animation::GetCurrentFrameIndex() const {
    return currentFrameIndex;
}

const std::string& Animation::GetCurrentFrameName() const {
    static const std::string emptyString = "";
    
    if (frameNames.empty() || currentFrameIndex >= static_cast<int>(frameNames.size())) {
        return emptyString;
    }
    
    return frameNames[currentFrameIndex];
}

int Animation::GetFrameCount() const {
    return static_cast<int>(frameNames.size());
}

float Animation::GetProgress() const {
    if (frameNames.empty()) {
        return 0.0f;
    }
    
    // 计算总进度：当前帧索引 + 当前帧内进度
    float frameProgress = 0.0f;
    float currentDuration = GetCurrentFrameDuration();
    if (currentDuration > 0.0f) {
        frameProgress = currentFrameTime / currentDuration;
    }
    
    float totalProgress = (currentFrameIndex + frameProgress) / frameNames.size();
    return std::clamp(totalProgress, 0.0f, 1.0f);
}

void Animation::SetCurrentFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < static_cast<int>(frameNames.size())) {
        currentFrameIndex = frameIndex;
        currentFrameTime = 0.0f;
        isFinished = false;
    }
}

void Animation::SetCurrentFrameByName(const std::string& frameName) {
    auto it = std::find(frameNames.begin(), frameNames.end(), frameName);
    if (it != frameNames.end()) {
        currentFrameIndex = static_cast<int>(std::distance(frameNames.begin(), it));
        currentFrameTime = 0.0f;
        isFinished = false;
    }
}