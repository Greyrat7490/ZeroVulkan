#ifndef RENDERER_H_
#define RENDERER_H_

#include <functional>
#include "Vulkan/Core.h"
#include "ZScene/ZScene.h"
#include "ZObject/ZObject.h"

namespace ZeroVulkan::ZRenderer {
    void printVulkanInfos();
   
    void record();

    void initRenderer();
   
    void resizing();
    
    void drawFrame();

    void update(float dt);

    void clear();

    void updateWinSize();
}

#endif // RENDERER_H_
