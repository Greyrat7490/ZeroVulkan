#ifndef ZOBJECT_SCENE_H_
#define ZOBJECT_SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

using namespace ZeroVulkan;

struct ObjUBO {
    mat4 proj;
    mat4 view;
    mat4 model;
    vec3 lightDir;
};

class ZObjectScene : public ZeroVulkan::ZScene 
{
public:
    ZObjectScene() = default;
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
    
    ObjUBO* objUbo = nullptr;
};

#endif // ZOBJECT_SCENE_H_
