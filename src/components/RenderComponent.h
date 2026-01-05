//
// Created by zream on 2026/1/5.
//

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <memory>
#include <string>

#include "../graphics/Sprite.h"
#include "../graphics/SpriteAtlas.h"
#include "../graphics/Renderer.h"
#include "../animation/Animator.h"
#include "../entity/EntityBase.h"

/**
 * RenderComponent - 实体渲染组件
 *
 * 职责：
 * - 持有与该实体相关的渲染资源（Sprite）
 * - 可选地关联 SpriteAtlas 和 Animator
 * - 根据实体的位置/尺寸与 Animator 当前帧，完成渲染
 * - 负责纹理资源的生命周期（加载/释放）
 *
 * 不负责：
 * - 游戏逻辑（交由实体自身）
 * - 动画状态决策（交由 Animator）
 * - 图集数据解析（交由 SpriteAtlas）
 */
class RenderComponent {
public:
    RenderComponent();
    ~RenderComponent();

    // 绑定所属实体（非拥有指针，仅用于读取位置/尺寸等）
    void SetOwner(EntityBase* ownerEntity);

    // 绑定 Animator（非拥有指针，仅用于读取当前帧名称或索引）
    void SetAnimator(Animator* animatorPtr);

    // 绑定 SpriteAtlas（非拥有指针，仅用于根据帧名查询裁剪矩形）
    void SetSpriteAtlas(const SpriteAtlas* atlasPtr);

    // 资源初始化
    // 直接从纹理路径加载（不使用图集）
    bool LoadTexture(Renderer* renderer, const std::string& texturePath);

    // 使用 SpriteAtlas 的纹理路径加载（推荐与图集配合使用）
    bool LoadTextureFromAtlas(Renderer* renderer);

    // 设置在没有 Animator 时使用的默认帧名称（需要 SpriteAtlas 支持）
    void SetDefaultFrameName(const std::string& frameName);

    // 允许外部注入已加载的共享 Sprite，以便多个组件共享纹理
    void SetSprite(const std::shared_ptr<Sprite>& spritePtr);

    // 显示控制
    void SetVisible(bool visible);
    bool IsVisible() const;

    // 更新逻辑（预留，用于将来做闪烁、渐变等效果）
    void Update(float deltaTime);

    // 渲染：根据 owner 的位置和 Animator 当前帧进行绘制
    void Render(Renderer* renderer);

    // 直接访问底层 Sprite（用于特殊渲染需求）
    Sprite* GetSprite();
    const Sprite* GetSprite() const;

private:
    EntityBase* owner;              // 所属实体（非拥有）
    Animator* animator;             // 动画控制器（非拥有）
    const SpriteAtlas* spriteAtlas; // 精灵图集（非拥有）

    std::shared_ptr<Sprite> sprite; // 纹理资源（可共享）
    bool isVisible;                 // 是否可见

    std::string defaultFrameName;   // 默认帧名称（在没有 Animator 时使用）

    // 内部辅助：根据 Animator 和 SpriteAtlas 计算当前源矩形
    bool GetCurrentSourceRect(SDL_Rect& outRect) const;
};

#endif //RENDERCOMPONENT_H
