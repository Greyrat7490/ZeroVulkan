#ifndef H_DEPTH_BUFFERING
#define H_DEPTH_BUFFERING

#include "Device.h"
#include "Image.h"

namespace ZeroVulkan
{
	inline void initDepthBuffering()
	{
		createImage(ZDevice::getDepthImage(), ZDevice::getDepthImageMemory(), ZDevice::getSwapchainExtent().width, ZDevice::getSwapchainExtent().height, ZDevice::getDepthFormat(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		createImageView(ZDevice::getDepthImageView(), ZDevice::getDepthImage(), ZDevice::getDepthFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);
	}
}

#endif // H_DEPTH_BUFFERING
