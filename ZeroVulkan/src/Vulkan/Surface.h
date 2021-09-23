#ifndef H_SURFACE
#define H_SURFACE

#include <stdio.h>
#include "Device.h"
#include <xcb/xcb.h>

namespace ZeroVulkan
{
	inline void createXcbSurface(xcb_connection_t* connection, xcb_window_t window)
	{
        VkXcbSurfaceCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        info.connection = connection;
        info.window = window;
        
        vkCreateXcbSurfaceKHR(ZDevice::getInstance(), &info, nullptr, &ZDevice::getSurface());
	}

	inline void chooseSurfaceFormat() {
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &formatCount, nullptr);

		std::vector<VkSurfaceFormatKHR> availableFormats;

		if(formatCount)
		{
			availableFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &formatCount, availableFormats.data());
		}
		
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				ZDevice::getSurfaceFormat() = availableFormat;
				return;
			}
		}

		ZDevice::getSurfaceFormat() = availableFormats[0];
	}

	inline void checkSurfaceSupport()
	{
		VkBool32 surfaceSupport = false;
		VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(ZDevice::getPhysicalDev()[0], 0, ZDevice::getSurface(), &surfaceSupport);
        if (res != VK_SUCCESS)
            printf("get physical dev surface support ERROR: %d\n", res);

		if (!surfaceSupport)
			printf("Surface not supported!\n");
		else
			printf("Surface is supported!\n");

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &ZDevice::getSurfaceCapabilities());
		
		chooseSurfaceFormat();
	}
	
	inline void setSurfaceExtent(uint32_t width, uint32_t height)
	{
		VkExtent2D extent;

		// update Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &ZDevice::getSurfaceCapabilities());

		if (width <= ZDevice::getSurfaceCapabilities().maxImageExtent.width
			&& width >= ZDevice::getSurfaceCapabilities().minImageExtent.width)
			extent.width = width;
		else
			extent.width = ZDevice::getSurfaceCapabilities().maxImageExtent.width;

		if (height <= ZDevice::getSurfaceCapabilities().maxImageExtent.height
			&& height >= ZDevice::getSurfaceCapabilities().minImageExtent.height)
			extent.height = height;
		else
			extent.height = ZDevice::getSurfaceCapabilities().maxImageExtent.height;

		ZDevice::getSwapchainExtent() = extent;
	}
}

#endif // H_SURFACE
