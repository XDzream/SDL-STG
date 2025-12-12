//
// Created by zream on 2025/12/11.
//

#ifndef BULLETCONFIG_H
#define BULLETCONFIG_H

#include <string>
#include <vector>
#include <SDL3/SDL.h>

/**
 * 子弹配置结构体
 * 用于存储从JSON文件读取的子弹外观和碰撞体信息
 * 不包含行为参数（速度、寿命等），这些由角色/发射器逻辑控制
 */
struct BulletConfig {
    // 子弹唯一标识符，用于在配置管理器中查找
    std::string id;
    
    // 贴图文件路径（相对或绝对路径）
    std::string texture;
    
    // 精灵表中的帧列表（不规则裁剪）
    // 每个SDL_Rect表示一个帧的裁剪区域：x, y, w, h
    // 如果只有一帧，可以只放一个元素；多帧用于动画或不同状态
    std::vector<SDL_Rect> frames;
    
    // 碰撞体配置
    struct {
        // 碰撞体类型："circle"（圆形）或 "rect"（矩形）
        std::string type;
        
        // 圆形碰撞体半径（type == "circle" 时使用）
        float radius;
        
        // 矩形碰撞体的宽度和高度（type == "rect" 时使用）
        float w, h;
    } collider;
    
    // 渲染缩放倍数（默认1.0表示原始大小）
    // 可以用于放大或缩小子弹的显示尺寸
    float renderScale;
    
    // 默认构造函数，初始化默认值
    BulletConfig() : renderScale(1.0f) {
        collider.type = "circle";
        collider.radius = 0.0f;
        collider.w = collider.h = 0.0f;
    }
};

#endif //BULLETCONFIG_H
