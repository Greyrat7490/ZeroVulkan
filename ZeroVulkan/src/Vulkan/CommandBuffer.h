#ifndef H_COMMAND_BUFFER
#define H_COMMAND_BUFFER

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <vector>

namespace ZeroVulkan
{
    class ZCommandPool
    {
    public:
        ZCommandPool();
        ~ZCommandPool();

        inline VkCommandPool& getPool() { return m_commandPool; }
        inline const std::vector<VkCommandBuffer>& getBuffers() const { return m_commandBuffers; }
    
        VkCommandBuffer beginCommand();
        void endCommand(VkQueue queue, VkCommandBuffer commandBuffer);
        void createCommandBuffers(uint32_t bufferCount);
    private:
        VkCommandPool m_commandPool = nullptr;
        std::vector<VkCommandBuffer> m_commandBuffers;
    };
}

#endif // H_COMMAND_BUFFER
