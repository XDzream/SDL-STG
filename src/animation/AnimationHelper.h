//
// Created by zream on 2026/1/5.
//

#ifndef ANIMATIONHELPER_H
#define ANIMATIONHELPER_H

#include "Animator.h"
#include "../graphics/SpriteAtlas.h"
#include <string>

/**
 * AnimationHelper - 动画加载辅助工具
 * 职责：
 * - 从 SpriteAtlas 加载动画配置到 Animator
 * - 解耦 Animator 和 SpriteAtlas 的直接依赖
 */
class AnimationHelper {
public:
    /**
     * 从 SpriteAtlas 加载动画到 Animator
     * @param animator 目标动画控制器
     * @param atlas 精灵图集
     * @param stateName 动画状态名称
     * @param animationName 在 Atlas 中的动画名称
     * @param loop 是否循环
     * @return 是否成功加载
     */
    static bool LoadAnimationFromAtlas(Animator& animator, 
                                       const SpriteAtlas& atlas,
                                       const std::string& stateName, 
                                       const std::string& animationName,
                                       bool loop = true);
    
    /**
     * 批量加载动画（状态名和动画名相同）
     * @param animator 目标动画控制器
     * @param atlas 精灵图集
     * @param animationNames 动画名称列表
     * @param loop 是否循环
     * @return 成功加载的动画数量
     */
    static int LoadAnimationsFromAtlas(Animator& animator,
                                       const SpriteAtlas& atlas,
                                       const std::vector<std::string>& animationNames,
                                       bool loop = true);
};

#endif //ANIMATIONHELPER_H
