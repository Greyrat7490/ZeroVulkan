#ifndef H_MEMORY
#define H_MEMORY

#define VK_USE_PLATFORM_XCB_KHR
#include <stdio.h>
#include <vulkan/vulkan.h>

namespace ZeroVulkan
{
    inline uint32_t findMemTypeIndex(VkPhysicalDevice& physicalDev, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties devMemPropes;
        vkGetPhysicalDeviceMemoryProperties( physicalDev, &devMemPropes);

        for (uint32_t i = 0; i < devMemPropes.memoryTypeCount; i++)
        {
            if (typeFilter & (1 << i) && devMemPropes.memoryTypes[i].propertyFlags & properties)
                return i;
        }

        printf( "ERR: Could no find a correct memory type!\n");

        return 0;
    }

    template<typename T>
    inline void mapMemory(T src, VkDevice dev, VkDeviceMemory dst, VkDeviceSize offset)
    {
        void* data;
        vkMapMemory(dev, dst, offset, sizeof(T), 0, &data);
        memcpy(data, &src, sizeof(T));
        vkUnmapMemory(dev, dst);
    }
}

#endif // H_MEMORY
