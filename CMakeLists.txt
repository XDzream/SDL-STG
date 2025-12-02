cmake_minimum_required(VERSION 3.30)
project(NewSdlButtleHell)

set(CMAKE_CXX_STANDARD 20)

# SDL3路径配置
set(SDL3_DIR F:/123/StackingArea/SDL3-3.2.10/x86_64-w64-mingw32)
include_directories(${SDL3_DIR}/include)
link_directories(${SDL3_DIR}/bin)

# 收集源文件
file(GLOB_RECURSE SOURCES
        "src/*.cpp"     # 推荐使用src目录
        "src/*.c"
)

file(GLOB_RECURSE HEADERS
        "src/*.h"
        "src/*.hpp"
)

# 过滤掉cmake-build-debug目录中的文件
list(FILTER SOURCES EXCLUDE REGEX ".*cmake-build-debug.*")
list(FILTER HEADERS EXCLUDE REGEX ".*cmake-build-debug.*")


# 创建可执行文件
add_executable(NewSdlButtleHell
        ${SOURCES}
        ${HEADERS}
        src/graphics/Renderer.cpp
        src/graphics/Renderer.h
        src/input/InputHandler.cpp
        src/input/InputHandler.h
        src/graphics/Sprite.cpp
        src/graphics/Sprite.h
)

# 链接SDL3库
target_link_libraries(NewSdlButtleHell mingw32 SDL3 SDL3_image SDL3_ttf)
