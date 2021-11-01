#ifndef SCENE_H_
#define SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

struct ParticleSystemUBO
{
    ZeroVulkan::vec4 startColor;
    ZeroVulkan::vec4 endColor;
    ZeroVulkan::vec4 startPos;
    float startSize;
    float endSize;
    float lifeTime;
    float spawnDelay;
    float deltaTime;
};

class TestScene : public ZeroVulkan::ZScene 
{
public:
    TestScene() = default;
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
    
    ParticleSystemUBO* ubo = nullptr;
};

#endif // SCENE_H_
