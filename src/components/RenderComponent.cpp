//
// Created by zream on 2026/1/5.
//

#include "RenderComponent.h"

#include <iostream>

RenderComponent::RenderComponent()
    : owner(nullptr),
      animator(nullptr),
      spriteAtlas(nullptr),
      sprite(nullptr),
      isVisible(true),
      defaultFrameName("") {
}

RenderComponent::~RenderComponent() = default;

void RenderComponent::SetOwner(EntityBase* ownerEntity) {
    owner = ownerEntity;
}

void RenderComponent::SetAnimator(Animator* animatorPtr) {
    animator = animatorPtr;
}

void RenderComponent::SetSpriteAtlas(const SpriteAtlas* atlasPtr) {
    spriteAtlas = atlasPtr;
}

bool RenderComponent::LoadTexture(Renderer* renderer, const std::string& texturePath) {
    if (!renderer) {
        std::cerr << "RenderComponent::LoadTexture: renderer is null" << std::endl;
        return false;
    }

    auto newSprite = std::make_shared<Sprite>();
    if (!newSprite->LoadFromFile(texturePath, *renderer)) {
        std::cerr << "RenderComponent::LoadTexture: failed to load texture: "
                  << texturePath << std::endl;
        return false;
    }

    sprite = std::move(newSprite);
    return true;
}

bool RenderComponent::LoadTextureFromAtlas(Renderer* renderer) {
    if (!spriteAtlas) {
        std::cerr << "RenderComponent::LoadTextureFromAtlas: spriteAtlas is null" << std::endl;
        return false;
    }

    const std::string& texturePath = spriteAtlas->GetTexturePath();
    if (texturePath.empty()) {
        std::cerr << "RenderComponent::LoadTextureFromAtlas: texture path is empty" << std::endl;
        return false;
    }

    return LoadTexture(renderer, texturePath);
}

void RenderComponent::SetDefaultFrameName(const std::string& frameName) {
    defaultFrameName = frameName;
}

void RenderComponent::SetSprite(const std::shared_ptr<Sprite>& spritePtr) {
    sprite = spritePtr;
}

void RenderComponent::SetVisible(bool visible) {
    isVisible = visible;
}

bool RenderComponent::IsVisible() const {
    return isVisible;
}

void RenderComponent::Update(float /*deltaTime*/) {
    // 预留：未来可以在这里做闪烁、渐隐/渐现等特效
}

void RenderComponent::Render(Renderer* renderer) {
    if (!isVisible) {
        return;
    }

    if (!renderer) {
        std::cerr << "RenderComponent::Render: renderer is null" << std::endl;
        return;
    }

    if (!sprite || !sprite->IsLoaded()) {
        // 没有纹理就不渲染
        return;
    }

    if (!owner) {
        std::cerr << "RenderComponent::Render: owner is null" << std::endl;
        return;
    }

    // 从实体获取位置和尺寸
    float x = owner->GetX();
    float y = owner->GetY();
    float w = owner->GetWidth();
    float h = owner->GetHeight();

    // 计算源矩形（如果有 Animator + SpriteAtlas）
    SDL_Rect srcRect{};
    bool hasSrc = GetCurrentSourceRect(srcRect);

    if (hasSrc) {
        sprite->Render(*renderer,
                       static_cast<int>(x),
                       static_cast<int>(y),
                       static_cast<int>(w),
                       static_cast<int>(h),
                       &srcRect);
    } else {
        // 没有源矩形时，渲染整张纹理
        sprite->Render(*renderer,
                       static_cast<int>(x),
                       static_cast<int>(y),
                       static_cast<int>(w),
                       static_cast<int>(h),
                       nullptr);
    }
}

Sprite* RenderComponent::GetSprite() {
    return sprite.get();
}

const Sprite* RenderComponent::GetSprite() const {
    return sprite.get();
}

bool RenderComponent::GetCurrentSourceRect(SDL_Rect& outRect) const {
    if (!spriteAtlas) {
        // 没有图集，无法提供裁剪信息
        return false;
    }

    std::string frameName;

    // 优先使用 Animator 当前帧
    if (animator) {
        frameName = animator->GetCurrentFrameName();
    }

    // 如果 Animator 没有提供有效帧，则使用默认帧
    if (frameName.empty()) {
        frameName = defaultFrameName;
    }

    if (frameName.empty()) {
        // 既没有动画帧，也没有默认帧
        return false;
    }

    if (!spriteAtlas->HasFrame(frameName)) {
        std::cerr << "RenderComponent::GetCurrentSourceRect: frame not found: "
                  << frameName << std::endl;
        return false;
    }

    outRect = spriteAtlas->GetFrameRect(frameName);
    return true;
}
