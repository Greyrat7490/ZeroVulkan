#ifndef ZERO_VULKAN_H_
#define ZERO_VULKAN_H_

#include "renderer.h"
#include "Vulkan/ZvObject.h"
#include "Vulkan/types.h"

namespace ZeroVulkan {
    void init();
    void clear();
    
    void draw();
    bool handleInput();
    void update(float dt);
}

#endif // ifndef ZERO_VULKAN_H_
