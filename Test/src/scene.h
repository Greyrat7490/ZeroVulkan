#ifndef SCENE_H_
#define SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

class TestScene : public ZeroVulkan::ZScene 
{
public:
    TestScene();
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
};

#endif // SCENE_H_
