#ifndef ZCOMPUTE_SHADER_SCENE_H_
#define ZCOMPUTE_SHADER_SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

using namespace ZeroVulkan;

struct ParticleSystemUBO
{
    vec4 startColor;
    vec4 endColor;
    vec4 startPos;
    float startSize;
    float endSize;
    float lifeTime;
    float spawnDelay;
    float deltaTime;
};

class ComputeShaderScene : public ZeroVulkan::ZScene 
{
public:
    ComputeShaderScene() = default;
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
    
    ParticleSystemUBO* psUbo = nullptr;
};

#endif // ZCOMPUTE_SHADER_SCENE_H_
