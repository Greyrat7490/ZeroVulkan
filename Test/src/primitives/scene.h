#ifndef PRIMITIVES_SCENE_H_
#define PRIMITIVES_SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

using namespace ZeroVulkan;

class PrimitivesScene : public ZeroVulkan::ZScene 
{
public:
    PrimitivesScene() = default;
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
};

#endif // PRIMITIVES_SCENE_H_
