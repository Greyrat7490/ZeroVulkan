#ifndef ZERO_VULKAN_H_
#define ZERO_VULKAN_H_

#include "ZRenderer/ZRenderer.h"
#include "ZObject/ZObject.h"
#include "ZMesh/ZMesh.h"
#include "ZShaders/ZShaders.h"

namespace ZeroVulkan {
    void init();
    void clear();
    
    void draw();
    bool handleInput();
    void update(float dt);
}

#endif // ZERO_VULKAN_H_
