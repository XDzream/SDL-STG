//
// Created by zream on 2025/12/2.
//

#ifndef RENDERER_H
#define RENDERER_H

#endif //RENDERER_H

#include<SDL3/SDL.h>

class Renderer {

private:
  SDL_Window* window;

  SDL_Renderer* renderer;

  bool isInitialized;

public:
  Renderer();

  ~Renderer();
  //核心功能函数
  bool Initialize(const char* title, int width, int height);
  void Cleanup();
  
  //渲染函数
  void Clear();
  void Present();
  void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

  //状态查询函数
  [[nodiscard]] bool IsInitialized() const;
  [[nodiscard]] SDL_Window* GetWindow() const;
  [[nodiscard]] SDL_Renderer* GetRenderer() const;


};