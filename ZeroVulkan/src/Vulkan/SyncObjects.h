#ifndef H_SYNC_OBJS
#define H_SYNC_OBJS

#include <stdio.h>
#include "Device.h"

// TODO: better sync (use fence)
namespace ZeroVulkan::SyncObjects
{
    VkSemaphore* getSemaphoreImgAvailable();
    VkSemaphore* getSemaphoreRenderingDone();
     
	void create(uint32_t maxFramesInFlight);
    void clear();
}

#endif // H_SYNC_OBJS
