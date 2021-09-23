#ifndef RENDERER_H_
#define RENDERER_H_

#include "Vulkan/Core.h"
#include "Vulkan/Scene.h"
#include "Vulkan/ZvObject.h"

namespace ZeroVulkan::ZRenderer {
    void printVulkanInfos();

    void updateWindowSize();

    void updateProj();

    void record();

    void initRenderer();

    void refreshSwapchain();

    void drawFrame();

    void update(float dt);

    size_t createObject();
}

#endif // RENDERER_H_
