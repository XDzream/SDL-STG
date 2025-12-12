//
// Created by zream on 2025/12/11.
//

#ifndef BULLETCONFIGPARSER_H
#define BULLETCONFIGPARSER_H

#include <string>
#include "BulletConfig.h"

/**
 * 子弹配置解析器
 * 负责从JSON文件读取并解析BulletConfig
 */
class BulletConfigParser {
public:
    /**
     * 从JSON文件加载并解析子弹配置
     * @param filePath JSON文件路径
     * @param config 输出的配置对象（引用传递，会被填充）
     * @return 成功返回true，失败返回false
     */
    static bool LoadFromFile(const std::string& filePath, BulletConfig& config);
    
    /**
     * 从JSON字符串解析配置（可选，用于测试或内存中的JSON）
     * @param jsonString JSON字符串
     * @param config 输出的配置对象
     * @return 成功返回true，失败返回false
     */
    static bool LoadFromString(const std::string& jsonString, BulletConfig& config);

private:
    // 辅助方法：解析单个帧的SDL_Rect
    static SDL_Rect ParseFrameRect(int x, int y, int w, int h);
};

#endif //BULLETCONFIGPARSER_H

