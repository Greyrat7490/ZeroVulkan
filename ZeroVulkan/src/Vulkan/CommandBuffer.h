#ifndef COMMAND_BUFFER_H_
#define COMMAND_BUFFER_H_

#include <vulkan/vulkan_core.h>
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

#endif // COMMAND_BUFFER_H_
