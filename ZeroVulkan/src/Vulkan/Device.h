#ifndef DEVICE_H_
#define DEVICE_H_

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>

#include <vector>
#include <stdio.h>

#include "CommandBuffer.h"


// static struct QueueFamilyIndices
// {
//     int graphics = -1;
//     int present = -1;
//     int compute = -1;
// };

namespace ZeroVulkan::ZDevice {
    void init();
    void clear();
    
    VkDevice getDevice();
    VkInstance getInstance();
    VkPhysicalDevice* getPhysicalDev();
    VkQueue& getQueue();
    ZCommandPool* getCommandPool();

    VkSampler& getSampler();
}

#endif // DEVICE_H_
