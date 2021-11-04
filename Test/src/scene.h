#ifndef SCENE_H_
#define SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

using namespace ZeroVulkan;

struct ObjUBO {
    mat4 proj;
    mat4 view;
    mat4 model;
    vec3 lightDir;
};

struct OutlineUBO {
    mat4 proj;
    mat4 view;
    mat4 model;
};

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

class TestScene : public ZeroVulkan::ZScene 
{
public:
    TestScene() = default;
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
    
    ParticleSystemUBO* psUbo = nullptr;
    ObjUBO* objUbo = nullptr;
    OutlineUBO* outlineUbo = nullptr;
};

#endif // SCENE_H_
