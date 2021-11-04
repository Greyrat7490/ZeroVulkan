#include "CommandBuffer.h"

#include <cstdio>
#include "Device.h"

namespace ZeroVulkan
{
    VkCommandBuffer ZCommandPool::beginCommand() {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        allocInfo.commandPool = m_commandPool;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(ZDevice::getDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void ZCommandPool::endCommand(VkQueue queue, VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(ZDevice::getDevice(), m_commandPool, 1, &commandBuffer);
    }

    void ZCommandPool::createCommandBuffers(uint32_t bufferCount)
    {
        m_commandBuffers.resize(bufferCount);

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = m_commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

        VkResult res = vkAllocateCommandBuffers(ZDevice::getDevice(), &commandBufferAllocateInfo, m_commandBuffers.data());
        if (res != VK_SUCCESS)
            printf("allocate command buffer ERROR: %d\n", res);

        printf("created %d commandBuffers\n", bufferCount);
    }

    ZCommandPool::ZCommandPool()
    {
        // TODO: one command pool per queueFamily?
        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = 0;

        VkResult res = vkCreateCommandPool(ZDevice::getDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool);
        if(res != VK_SUCCESS)
            printf("create command pool ERROR: %d\n", res);
        else
            printf("created ZCommandPool\n");
    }

    ZCommandPool::~ZCommandPool()
    {
        vkFreeCommandBuffers(ZDevice::getDevice(), m_commandPool, (uint32_t) m_commandBuffers.size(), m_commandBuffers.data());
        vkDestroyCommandPool(ZDevice::getDevice(), m_commandPool, nullptr);

        printf("destroyed ZCommandPool\n");
    }
}
