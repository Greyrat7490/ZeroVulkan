#ifndef SURFACE_H_
#define SURFACE_H_

#include <vulkan/vulkan_core.h>
#include <X11/Xlib.h>

namespace ZeroVulkan::Surface {
    VkSurfaceKHR getSurface();
    VkSurfaceFormatKHR getSurfaceFormat();
    const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities();


    void createXlibSurface(Display* dpy, Window window);
    void clear();

    void checkSurfaceSupport();
    VkExtent2D getCapableExtent(VkExtent2D extent);
}

#endif // SURFACE_H_
