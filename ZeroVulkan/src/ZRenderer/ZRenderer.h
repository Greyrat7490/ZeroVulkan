#ifndef RENDERER_H_
#define RENDERER_H_

#include <functional>
#include "Vulkan/Core.h"
#include "ZScene/ZScene.h"
#include "ZObject/ZObject.h"

namespace ZeroVulkan::ZRenderer {
    void printVulkanInfos();

    void updateWindowSize();

    void updateProj();

    void addBindFunction(std::function<void(VkCommandBuffer&)> func);
    
    void record();

    void initRenderer();

    void refreshSwapchain();

    void drawFrame();

    void update(float dt);

    size_t createObject();

    void clear();
}

#endif // RENDERER_H_
