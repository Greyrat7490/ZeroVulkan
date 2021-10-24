#ifndef H_SURFACE
#define H_SURFACE

#include <stdio.h>
#include "Device.h"
#include <xcb/xcb.h>

namespace ZeroVulkan::Surface {
    const VkSurfaceKHR& getSurface();
    const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities();
    const VkSurfaceFormatKHR& getSurfaceFormat();
    
    void clear();
    
	void createXcbSurface(xcb_connection_t* connection, xcb_window_t window);
	void checkSurfaceSupport();
	VkExtent2D getCapableExtent(VkExtent2D extent);
}

#endif // H_SURFACE
