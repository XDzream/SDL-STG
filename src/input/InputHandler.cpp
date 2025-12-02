//
// Created by zream on 2025/12/2.
//

#include "InputHandler.h"

InputHandler::InputHandler() {
    currentKeyStates = nullptr;
    memset(previousKeyStates, 0, sizeof(previousKeyStates));
}

void InputHandler::Update() {
    // 保存当前状态到历史状态
    if (currentKeyStates) {
        memcpy(previousKeyStates, currentKeyStates, sizeof(previousKeyStates));
    }
    
    // 获取最新的键盘状态
    currentKeyStates = SDL_GetKeyboardState(nullptr);
}

bool InputHandler::IsKeyPressed(SDL_Keycode key) const {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key,nullptr);
    return currentKeyStates[scancode];
}

bool InputHandler::IsKeyJustPressed(SDL_Keycode key) const{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key,nullptr);
    return currentKeyStates[scancode] && !previousKeyStates[scancode];
}

bool InputHandler::IsKeyJustReleased(SDL_Keycode key) const{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key,nullptr);
    return !currentKeyStates[scancode] && previousKeyStates[scancode];
}


