//
// Created by zream on 2026/1/5.
//

#include "AnimationHelper.h"
#include <iostream>

bool AnimationHelper::LoadAnimationFromAtlas(Animator& animator, 
                                             const SpriteAtlas& atlas,
                                             const std::string& stateName, 
                                             const std::string& animationName,
                                             bool loop) {
    // 从 SpriteAtlas 获取动画帧序列
    std::vector<std::string> frameNames = atlas.GetAnimationFrames(animationName);
    if (frameNames.empty()) {
        std::cerr << "AnimationHelper: 未找到动画 '" << animationName << "'" << std::endl;
        return false;
    }
    
    // 从 Atlas 获取每帧的持续时间
    std::vector<float> durations;
    for (const auto& frameName : frameNames) {
        float duration = atlas.GetFrameDuration(frameName);
        durations.push_back(duration);
    }
    
    // 添加到 Animator
    animator.AddAnimation(stateName, frameNames, durations, loop);
    return true;
}

int AnimationHelper::LoadAnimationsFromAtlas(Animator& animator,
                                             const SpriteAtlas& atlas,
                                             const std::vector<std::string>& animationNames,
                                             bool loop) {
    int successCount = 0;
    for (const auto& name : animationNames) {
        if (LoadAnimationFromAtlas(animator, atlas, name, name, loop)) {
            successCount++;
        }
    }
    return successCount;
}
