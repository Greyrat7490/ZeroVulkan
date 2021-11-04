#ifndef SYNC_OBJS_H_
#define SYNC_OBJS_H_

#include <vulkan/vulkan_core.h>

// TODO: better sync (use fence)
namespace ZeroVulkan::SyncObjects
{
    VkSemaphore* getSemaphoreImgAvailable();
    VkSemaphore* getSemaphoreRenderingDone();

    void create(uint32_t maxFramesInFlight);
    void clear();
}

#endif // SYNC_OBJS_H_
