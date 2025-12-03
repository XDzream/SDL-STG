//
// Created by zream on 2025/12/2.
//

#ifndef GAME_H
#define GAME_H



#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <memory>

#include "../graphics/Renderer.h"
#include "../graphics/Sprite.h"
#include "../input/InputHandler.h"

#endif //GAME_H

class Game {


public:
    Game();

    ~Game();


    int Run();



private:
    //核心函数
    std::unique_ptr<Renderer> gameRenderer;
    std::unique_ptr<InputHandler> gameInputHandler;
    std::unique_ptr<Sprite> gameSprite;

    
    //游戏状态
    bool gameRunning;


    //时间管理
    Uint64 lastFrameTime;
    Uint64 currentFrameTime;
    double deltaTime;

    //窗口
    const int windowWidth = 800;
    const int windowHeight = 600;
    const char* windowTitle = "Test window";

    //测试精灵位置
    float testSpriteX = 400.0f;
    float testSpriteY = 300.0f;

    // 初始化和清理
    bool Initialize();
    void Cleanup();
    
    // 游戏循环核心方法
    void Update();
    void Render();
    void HandleEvents();
    
    // 帧率控制
    void FrameRateControl();
    
    // 测试用的精灵移动逻辑（后续会被Player系统替代）
    void UpdateTestSprite();
    void RenderTestSprite();













};
