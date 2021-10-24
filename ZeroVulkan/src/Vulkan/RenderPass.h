#ifndef H_RENDERPASS
#define H_RENDERPASS

#include <vulkan/vulkan_core.h>

namespace ZeroVulkan::RenderPass {
    VkRenderPass* getRenderPass();
	void create(VkFormat format);
    void clear();
}

#endif // H_RENDERPASS
