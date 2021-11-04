#include "DepthBuffering.h"
#include "Swapchain.h"
#include "Image.h"

namespace ZeroVulkan::DepthBuffering {
    static const VkFormat s_format = VK_FORMAT_D32_SFLOAT;
    static VkImage s_image = nullptr;
    static VkImageView s_imageView = nullptr;
    static VkDeviceMemory s_imageMemory = nullptr;
 
    VkFormat getDepthFormat() { return s_format; }
    VkImage& getDepthImage() { return s_image; }
    VkImageView& getDepthImageView() { return s_imageView; }
    VkDeviceMemory& getDepthImageMemory() { return s_imageMemory; }
    
    void init()
    {
        createImage(s_image, s_imageMemory, Swapchain::getExtent().width, Swapchain::getExtent().height, s_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        createImageView(s_imageView, s_image, s_format, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    void clear() {
        vkDestroyImageView(ZDevice::getDevice(), s_imageView, nullptr);
        vkDestroyImage(ZDevice::getDevice(), s_image, nullptr);
        vkFreeMemory(ZDevice::getDevice(), s_imageMemory, nullptr);
    }
}
