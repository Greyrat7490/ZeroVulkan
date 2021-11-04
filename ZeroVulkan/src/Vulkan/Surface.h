#ifndef SURFACE_H_
#define SURFACE_H_

#include <xcb/xcb.h>
#include <vulkan/vulkan_core.h>

namespace ZeroVulkan::Surface {
    VkSurfaceKHR getSurface();
    VkSurfaceFormatKHR getSurfaceFormat();
    const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities();


    void createXcbSurface(xcb_connection_t* connection, xcb_window_t window);
    void clear();

    void checkSurfaceSupport();
    VkExtent2D getCapableExtent(VkExtent2D extent);
}

#endif // SURFACE_H_
