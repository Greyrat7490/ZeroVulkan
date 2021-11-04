#include "Surface.h"
#include "Vulkan/Device.h"
#include "Vulkan/Swapchain.h"

namespace ZeroVulkan::Surface {
    static VkSurfaceKHR s_surface;
    static VkSurfaceFormatKHR s_surfaceFormat;
    static VkSurfaceCapabilitiesKHR s_surfaceCapabilities;
    

    VkSurfaceKHR getSurface() { return s_surface; }
    VkSurfaceFormatKHR getSurfaceFormat() { return s_surfaceFormat; }
    const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() { return s_surfaceCapabilities; }


    void createXcbSurface(xcb_connection_t* connection, xcb_window_t window)
    {
        VkXcbSurfaceCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        info.connection = connection;
        info.window = window;
        
        vkCreateXcbSurfaceKHR(ZDevice::getInstance(), &info, nullptr, &s_surface);
    }

    void chooseSurfaceFormat() {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::getPhysicalDev()[0], s_surface, &formatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> availableFormats;

        if(formatCount)
        {
            availableFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::getPhysicalDev()[0], s_surface, &formatCount, availableFormats.data());
        }
        
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                s_surfaceFormat = availableFormat;
                return;
            }
        }

        s_surfaceFormat = availableFormats[0];
    }

    void checkSurfaceSupport()
    {
        VkBool32 surfaceSupport = false;
        VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(ZDevice::getPhysicalDev()[0], 0, s_surface, &surfaceSupport);
        if (res != VK_SUCCESS)
            printf("get physical dev surface support ERROR: %d\n", res);

        if (!surfaceSupport)
            printf("Surface not supported!\n");
        else
            printf("Surface is supported!\n");

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ZDevice::getPhysicalDev()[0], s_surface, &s_surfaceCapabilities);
        
        chooseSurfaceFormat();
    }
    
    VkExtent2D getCapableExtent(VkExtent2D extent)
    {
        VkExtent2D res;

        // update Capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ZDevice::getPhysicalDev()[0], s_surface, &s_surfaceCapabilities);

        if (extent.width <= s_surfaceCapabilities.maxImageExtent.width
            && extent.width >= s_surfaceCapabilities.minImageExtent.width)
            res.width = extent.width;
        else
            res.width = s_surfaceCapabilities.maxImageExtent.width;

        if (extent.height <= s_surfaceCapabilities.maxImageExtent.height
            && extent.height >= s_surfaceCapabilities.minImageExtent.height)
            res.height = extent.height;
        else
            res.height = s_surfaceCapabilities.maxImageExtent.height;

        return res;
    }

    void clear() {
        vkDestroySurfaceKHR(ZDevice::getInstance(), s_surface, nullptr);
    }
}

