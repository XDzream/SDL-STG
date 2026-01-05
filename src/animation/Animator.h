//
// Created by zream on 2026/1/4.
//

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

/**
 * Animator - 动画状态机（纯控制层）
 * 职责：
 * 1. 管理多个动画状态（IDLE, MOVE, ATTACK等）
 * 2. 根据游戏逻辑切换动画
 * 3. 提供当前帧名称给外部渲染
 * 4. 提供动画事件回调
 * 
 * 不负责：
 * - 不管理纹理（Sprite）
 * - 不管理图集数据（SpriteAtlas）
 * - 不执行渲染逻辑
 */
class Animator {
public:
    Animator();
    ~Animator() = default;
    
    // 动画注册：手动创建动画
    void AddAnimation(const std::string& stateName, const std::vector<std::string>& frameNames, 
                      float frameDuration = 0.1f, bool loop = true);
    
    // 动画注册：使用自定义帧持续时间
    void AddAnimation(const std::string& stateName, const std::vector<std::string>& frameNames, 
                      const std::vector<float>& frameDurations, bool loop = true);
    
    // 状态控制
    void Play(const std::string& stateName, bool forceRestart = false);
    void Stop();
    void Pause();
    void Resume();
    
    // 更新逻辑（必须在游戏循环中调用）
    void Update(float deltaTime);
    
    // 核心输出：获取当前帧名称（供外部查询SpriteAtlas或直接使用）
    const std::string& GetCurrentFrameName() const;
    int GetCurrentFrameIndex() const;
    
    // 状态查询
    bool IsPlaying() const;
    bool IsPaused() const;
    const std::string& GetCurrentState() const;
    bool HasAnimation(const std::string& stateName) const;
    float GetProgress() const;  // 当前动画播放进度 0.0-1.0
    
    // 高级功能：动画事件回调
    using AnimationCallback = std::function<void(const std::string&)>;
    void SetOnAnimationStart(AnimationCallback callback);     // 动画开始时
    void SetOnAnimationComplete(AnimationCallback callback);  // 动画完成时（仅非循环）
    void SetOnFrameChange(AnimationCallback callback);        // 帧变化时
    
    // 快捷配置
    void SetDefaultLoop(bool loop);  // 设置默认循环模式
    void SetSpeed(float speed);      // 设置全局播放速度
    
private:
    std::unordered_map<std::string, std::unique_ptr<Animation>> animations;  // 状态名 -> 动画
    std::string currentState;  // 当前动画状态
    std::string previousState; // 上一个状态（用于检测切换）
    
    Animation* currentAnimation;  // 当前正在播放的动画
    int lastFrameIndex;           // 上一帧的索引（用于检测帧变化）
    
    // 回调函数
    AnimationCallback onAnimationStart;
    AnimationCallback onAnimationComplete;
    AnimationCallback onFrameChange;
    
    // 配置
    bool defaultLoop;
    float globalSpeed;
    
    // 内部方法
    void SwitchToState(const std::string& stateName, bool forceRestart);
    void CheckAndTriggerCallbacks();
};

#endif //ANIMATOR_H
