//
// Created by zream on 2025/12/18.
// I just wanna say that the programme is so difficult.
//

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

/**
 * Animation - 纯粹的动画播放器
 * 职责：
 * 1. 管理帧序列（帧名称列表）
 * 2. 按时间推进当前帧索引
 * 3. 处理循环/单次播放
 * 4. 不管理纹理，不负责渲染
 */
class Animation {
public:
    Animation();
    ~Animation() = default;
    
    // 配置接口
    void SetFrames(const std::vector<std::string>& frameNames);
    void SetFrameDurations(const std::vector<float>& durations);  // 每帧持续时间（秒）
    void SetUniformDuration(float duration);  // 统一帧持续时间
    void SetLoop(bool loop);
    void SetSpeed(float speed);  // 播放速度倍率（1.0为正常速度）
    
    // 播放控制
    void Play();      // 从头开始播放
    void Resume();    // 恢复播放
    void Pause();     // 暂停
    void Stop();      // 停止并重置
    void Reset();     // 重置到第一帧
    
    // 更新逻辑（由外部驱动）
    void Update(float deltaTime);  // deltaTime单位：秒
    
    // 状态查询
    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsFinished() const;  // 非循环动画是否播放完毕
    int GetCurrentFrameIndex() const;
    const std::string& GetCurrentFrameName() const;
    int GetFrameCount() const;
    float GetProgress() const;  // 返回0.0-1.0的播放进度
    
    // 高级控制
    void SetCurrentFrame(int frameIndex);  // 直接跳转到某帧
    void SetCurrentFrameByName(const std::string& frameName);  // 按帧名跳转
    
private:
    std::vector<std::string> frameNames;      // 帧名称序列
    std::vector<float> frameDurations;        // 每帧持续时间（秒）
    
    int currentFrameIndex;      // 当前帧索引
    float currentFrameTime;     // 当前帧已播放时间
    float speed;                // 播放速度倍率
    
    bool isPlaying;             // 是否正在播放
    bool isPaused;              // 是否暂停
    bool isLoop;                // 是否循环播放
    bool isFinished;            // 是否播放完毕
    
    void AdvanceFrame();        // 推进到下一帧
    float GetCurrentFrameDuration() const;  // 获取当前帧持续时间
};

#endif //ANIMATION_H
