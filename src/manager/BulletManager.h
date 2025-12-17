//
// Created by zream on 2025/12/16.
//

//
// Created by zream on 2025/12/16.
//

#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <vector>
#include <memory>
#include <queue>
#include <unordered_set>
#include "../bullet/BulletFactory.h"
#include "../entity/BulletBase.h"
#include "../entity/EntityBase.h"
#include "../graphics/Renderer.h"

/**
 * 基于对象池的子弹管理器
 * 负责高效创建、更新、渲染和销毁所有子弹，以及处理碰撞检测
 */
class BulletManager {
public:
    BulletManager(size_t initialPoolSize = 1000, float expandFactor = 1.5f);
    ~BulletManager();

    // 初始化管理器
    bool Initialize(const std::string& configDir, Renderer& renderer);

    // 更新所有子弹
    void Update(float deltaTime);

    // 渲染所有子弹
    void Render(Renderer* renderer);

    // 重置子弹状态以便重用
    void ResetBulletState(BulletBase* bullet, BulletOwner owner, float x, float y);


    // 从对象池获取并创建子弹
    BulletBase* CreateBullet(const std::string& bulletType,
                            BulletOwner owner,
                            float x, float y);

    // 回收子弹到对象池
    void RecycleBullet(BulletBase* bullet);

    // 清除所有活跃子弹（保留在池中）
    void ClearActiveBullets();

    // 碰撞检测 - 检测子弹与实体的碰撞
    void CheckCollisions(std::vector<std::shared_ptr<EntityBase>>& entities);

    // 获取指定归属的所有子弹
    std::vector<BulletBase*> GetActiveBulletsByOwner(BulletOwner owner);

    // 获取当前活动子弹数量
    size_t GetActiveBulletCount() const;

    // 获取子弹工厂（用于查询子弹类型等）
    const BulletFactory* GetBulletFactory() const;

    // 性能统计
    size_t GetPoolSize() const;
    size_t GetAvailableBulletCount() const;

private:
    // 初始化对象池
    bool InitializeObjectPool(size_t size);

    // 扩展对象池
    bool ExpandObjectPool(size_t additionalSize);

    // 从池中获取可用子弹
    BulletBase* GetBulletFromPool();

    // 碰撞检测辅助函数
    void CheckBulletEntityCollisions(BulletBase* bullet,
                                     std::vector<std::shared_ptr<EntityBase>>& entities);
    
    // 子弹碰撞检测（可选，通常子弹之间不碰撞）
    void CheckBulletBulletCollisions();

    // 对象池管理
    std::vector<std::unique_ptr<BulletBase>> bulletPool;  // 对象池
    std::queue<size_t> availableIndices;                  // 可用对象索引队列
    std::unordered_set<BulletBase*> activeBullets;       // 活跃子弹集合

    // 子弹工厂
    std::unique_ptr<BulletFactory> bulletFactory;

    // 对象池配置
    size_t initialPoolSize;
    float expandFactor;
    size_t maxPoolSize;

    // 初始化状态
    bool initialized;
    
    // 性能统计
    size_t peakActiveCount;
    size_t totalCreatedCount;
};

#endif // BULLETMANAGER_H
