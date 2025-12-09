//
// Created by zream on 2025/12/2.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include<SDL3/SDL.h>
#include<iostream>
#include<cstring>


class InputHandler {

public:
    InputHandler();

    void Update();  // 更新输入状态

    bool IsKeyPressed(SDL_Keycode key) const;  // 检查按键状态

    bool IsKeyJustPressed(SDL_Keycode key) const;  // 检查刚按下

    bool IsKeyJustReleased(SDL_Keycode key) const;  // 检查刚释放
    
private:

    const bool* currentKeyStates;

    Uint8 previousKeyStates[SDL_SCANCODE_COUNT];
};

#endif //INPUTHANDLER_H
