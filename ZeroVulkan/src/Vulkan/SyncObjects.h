#ifndef H_SYNC_OBJS
#define H_SYNC_OBJS

#include <stdio.h>
#include "Device.h"

namespace ZeroVulkan
{
	inline void createSyncObjects(uint32_t maxFramesInFlight)
	{
		ZDevice::getFences().resize(maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		vkCreateSemaphore(ZDevice::getDevice(), &semaphoreInfo, nullptr, &ZDevice::getSemaphoreImgAvailable());
		vkCreateSemaphore(ZDevice::getDevice(), &semaphoreInfo, nullptr, &ZDevice::getSemaphoreRenderingDone());

		for(uint32_t i = 0; i < maxFramesInFlight; i++) 
		{
			if(vkCreateFence(ZDevice::getDevice(), &fenceInfo, nullptr, &ZDevice::getFences()[i]) != VK_SUCCESS)
				printf("ERROR: failed to create synchronization objects for a frame!\n");
		}
	}
}

#endif // H_SYNC_OBJS
