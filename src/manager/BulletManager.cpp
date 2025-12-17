//
// Created by zream on 2025/12/16.
//

#include "BulletManager.h"

#include <iostream>

BulletManager::BulletManager(size_t initialSize, float factor)
    : initialPoolSize(initialSize),
      expandFactor(factor),
      maxPoolSize(10000),  // 设置最大池大小限制
      initialized(false),
      peakActiveCount(0),
      totalCreatedCount(0) {
    bulletFactory = std::make_unique<BulletFactory>();
}

bool BulletManager::Initialize(const std::string& configDir, Renderer& renderer) {
    if (initialized) {
        std::cerr << "BulletManager already initialized" << std::endl;
        return false;
    }
    
    // 初始化子弹工厂
    if (!bulletFactory->Initialize(configDir, renderer)) {
        std::cerr << "Failed to initialize BulletFactory" << std::endl;
        return false;
    }
    
    // 初始化对象池
    if (!InitializeObjectPool(initialPoolSize)) {
        std::cerr << "Failed to initialize bullet object pool" << std::endl;
        return false;
    }
    
    initialized = true;
    std::cout << "BulletManager initialized with pool size: " << bulletPool.size() << std::endl;
    return true;
}

bool BulletManager::InitializeObjectPool(size_t size) {
    try {
        bulletPool.reserve(size);
        
        for (size_t i = 0; i < size; ++i) {
            // 创建子弹对象（使用默认构造函数）
            auto bullet = std::make_unique<BulletBase>(BulletOwner::PLAYER, 0, 0);
            // 将索引入队
            availableIndices.push(i);
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during object pool initialization: " << e.what() << std::endl;
        return false;
    }
}

BulletBase* BulletManager::CreateBullet(const std::string& bulletType,
                                         BulletOwner owner,
                                         float x, float y) {
    if (!initialized) {
        std::cerr << "BulletManager not initialized" << std::endl;
        return nullptr;
    }
    
    // 从对象池获取子弹
    BulletBase* bullet = GetBulletFromPool();
    if (!bullet) {
        // 尝试扩展池并再次获取
        if (ExpandObjectPool(static_cast<size_t>(bulletPool.size() * 0.2f))) {
            bullet = GetBulletFromPool();
        }
        
        if (!bullet) {
            std::cerr << "Bullet pool exhausted" << std::endl;
            return nullptr;
        }
    }
    
    // 重置子弹状态
    ResetBulletState(bullet, owner, x, y);
    
    // 使用BulletFactory初始化子弹（需要添加InitializeExistingBullet方法）
    if (!bulletFactory->InitializeExistingBullet(bullet, bulletType)) {
        std::cerr << "Failed to initialize bullet from config: " << bulletType << std::endl;
        RecycleBullet(bullet);
        return nullptr;
    }
    
    // 添加到活跃子弹集合
    activeBullets.insert(bullet);
    bullet->SetActive(true);
    
    // 更新统计
    totalCreatedCount++;
    if (activeBullets.size() > peakActiveCount) {
        peakActiveCount = activeBullets.size();
    }
    
    return bullet;
}


void BulletManager::RecycleBullet(BulletBase* bullet) {
    if (!bullet || !initialized) return;
    
    // 从活跃子弹集合中移除
    activeBullets.erase(bullet);
    
    // 重置子弹状态
    bullet->SetActive(false);
    bullet->SetVelocity(0, 0);
    bullet->SetLifeTime(0);
    
    // 查找子弹在池中的位置
    for (size_t i = 0; i < bulletPool.size(); ++i) {
        if (bulletPool[i].get() == bullet) {
            availableIndices.push(i);
            return;
        }
    }
    
    std::cerr << "Bullet not found in pool during recycling" << std::endl;
}


void BulletManager::Update(float deltaTime) {
    if (!initialized) return;
    
    // 使用迭代器安全遍历（允许在遍历过程中移除元素）
    for (auto it = activeBullets.begin(); it != activeBullets.end(); ) {
        BulletBase* bullet = *it;
        if (bullet && bullet->IsActive()) {
            bullet->Update(deltaTime);
            
            // 检查子弹是否应该被回收（过期或超出屏幕）
            if (bullet->IsExpired() || bullet->IsOutOfBounds(800, 600)) {
                RecycleBullet(bullet);
                it = activeBullets.erase(it);  // 获取下一个迭代器
            } else {
                ++it;  // 移动到下一个
            }
        } else {
            it = activeBullets.erase(it);  // 移除无效子弹
        }
    }
}

void BulletManager::Render(Renderer* renderer) {
    if (!initialized || !renderer) return;
    
    // 渲染所有活跃子弹
    for (BulletBase* bullet : activeBullets) {
        if (bullet && bullet->IsActive()) {
            bullet->Render(renderer);
        }
    }
}

void BulletManager::ResetBulletState(BulletBase* bullet, BulletOwner owner, float x, float y) {
    if (!bullet) return;
    
    // 重置位置
    bullet->SetPosition(x, y);
    
    // 重置运动状态
    bullet->SetVelocity(0, 0);
    bullet->SetAcceleration(0, 0);
    
    // 重置生命周期
    bullet->SetLifeTime(0);
    
    // 重置动画状态
    // 注意：BulletBase 类中没有直接设置动画帧的方法，需要添加或通过其他方式重置
    
    // 重置其他状态
    bullet->SetOwner(owner);
    bullet->SetDamage(1.0f); // 默认伤害值
    bullet->SetActive(true);
    
    // 重置碰撞体（使用默认碰撞体）
    // 注意：具体的碰撞体设置会在InitializeExistingBullet中重新配置
}


BulletBase* BulletManager::GetBulletFromPool() {
    if (availableIndices.empty()) {
        return nullptr;
    }
    
    size_t index = availableIndices.front();
    availableIndices.pop();
    
    return bulletPool[index].get();
}

bool BulletManager::ExpandObjectPool(size_t additionalSize) {
    size_t oldSize = bulletPool.size();
    size_t newSize = oldSize + additionalSize;
    
    if (newSize > maxPoolSize) {
        std::cerr << "Cannot expand pool beyond max size: " << maxPoolSize << std::endl;
        return false;
    }
    
    try {
        bulletPool.reserve(newSize);
        
        for (size_t i = oldSize; i < newSize; ++i) {
            // 创建新的子弹对象
            auto bullet = std::make_unique<BulletBase>(BulletOwner::PLAYER, 0, 0);
            availableIndices.push(i);
        }
        
        std::cout << "Expanded bullet pool from " << oldSize << " to " << newSize << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during pool expansion: " << e.what() << std::endl;
        return false;
    }
}

void BulletManager::CheckCollisions(std::vector<std::shared_ptr<EntityBase>>& entities) {
    if (!initialized) return;
    
    // 检查所有活跃子弹与实体的碰撞
    for (BulletBase* bullet : activeBullets) {
        if (bullet && bullet->IsActive()) {
            CheckBulletEntityCollisions(bullet, entities);
        }
    }
    
    // 检查子弹之间的碰撞（如果需要）
    CheckBulletBulletCollisions();
}

void BulletManager::CheckBulletEntityCollisions(BulletBase* bullet,
                                                 std::vector<std::shared_ptr<EntityBase>>& entities) {
    if (!bullet || !bullet->IsActive()) return;
    
    for (auto& entity : entities) {
        if (!entity || !entity->IsActive()) continue;
        
        // 跳过自身
        if (bullet == entity.get()) continue;
        
        // 检查碰撞
        if (bullet->IsCollidingWith(*entity)) {
            // 调用双方的碰撞处理函数
            bullet->OnCollision(entity.get());
            entity->OnCollision(bullet);
            
            // 如果子弹因碰撞而失效，提前退出
            if (!bullet->IsActive()) break;
        }
    }
}

std::vector<BulletBase*> BulletManager::GetActiveBulletsByOwner(BulletOwner owner) {
    std::vector<BulletBase*> result;
    
    for (BulletBase* bullet : activeBullets) {
        if (bullet && bullet->IsActive() && bullet->GetOwner() == owner) {
            result.push_back(bullet);
        }
    }
    
    return result;
}

size_t BulletManager::GetActiveBulletCount() const {
    return activeBullets.size();
}

size_t BulletManager::GetPoolSize() const {
    return bulletPool.size();
}

size_t BulletManager::GetAvailableBulletCount() const {
    return availableIndices.size();
}
