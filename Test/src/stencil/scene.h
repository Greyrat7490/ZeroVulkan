#ifndef ZSTENCIL_SCENE_H_
#define ZSTENCIL_SCENE_H_

#include "ZScene/ZScene.h"
#include "ZeroVulkan.h"

using namespace ZeroVulkan;

struct Obj2UBO {
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

class ZStencilScene : public ZeroVulkan::ZScene
{
public:
    ZStencilScene() = default;

    virtual void start() override;
    virtual void update(float dt) override;
    virtual void end() override;

    Obj2UBO* objUbo = nullptr;
    OutlineUBO* outlineUbo = nullptr;
    ZSandboxCam* cam = nullptr;
};

#endif // ZSTENCIL_SCENE_H_
