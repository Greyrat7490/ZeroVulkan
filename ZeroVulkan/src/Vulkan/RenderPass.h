#ifndef RENDER_PASS_H_
#define RENDER_PASS_H_

#include <vulkan/vulkan_core.h>

namespace ZeroVulkan::RenderPass {
    VkRenderPass* getRenderPass();

    void create(VkFormat format);
    void clear();
}

#endif // RENDER_PASS_H_
