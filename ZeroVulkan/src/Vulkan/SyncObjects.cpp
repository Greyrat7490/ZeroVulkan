#include "SyncObjects.h"

#include <cstdio>
#include "Device.h"

namespace ZeroVulkan::SyncObjects {
    static VkSemaphore s_semaphoreImgAvailable = nullptr;
    static VkSemaphore s_semaphoreRenderingDone = nullptr;
    // static std::vector<VkFence> inFlightFences = { nullptr };
    

    VkSemaphore* getSemaphoreImgAvailable() { return &s_semaphoreImgAvailable; }
    VkSemaphore* getSemaphoreRenderingDone() { return &s_semaphoreRenderingDone; }
    
    void create(uint32_t maxFramesInFlight)
    {
        (void) maxFramesInFlight;
        
        // inFlightFences.resize(maxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        // VkFenceCreateInfo fenceInfo = {};
        // fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        // fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        vkCreateSemaphore(ZDevice::getDevice(), &semaphoreInfo, nullptr, &s_semaphoreImgAvailable);
        vkCreateSemaphore(ZDevice::getDevice(), &semaphoreInfo, nullptr, &s_semaphoreRenderingDone);

        // for(uint32_t i = 0; i < maxFramesInFlight; i++)
        // {
        //     if(vkCreateFence(ZDevice::getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        //         printf("ERROR: failed to create synchronization objects for a frame!\n");
        // }
    }

    void clear() {
        // for (uint32_t i = 0; i < ZDevice::MAX_FRAMES_IN_FLIGHT; i++)
        //     vkDestroyFence(ZDevice::getDevice(), inFlightFences[i], nullptr);

        vkDestroySemaphore(ZDevice::getDevice(), s_semaphoreImgAvailable, nullptr);
        vkDestroySemaphore(ZDevice::getDevice(), s_semaphoreRenderingDone, nullptr);
    }
}

