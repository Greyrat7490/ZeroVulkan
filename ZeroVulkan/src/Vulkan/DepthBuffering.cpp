#include "DepthBuffering.h"
#include "Swapchain.h"
#include "Image.h"

namespace ZeroVulkan::DepthBuffering {
    const VkFormat m_format = VK_FORMAT_D32_SFLOAT;
    VkImage m_image = nullptr;
    VkImageView m_imageView = nullptr;
    VkDeviceMemory m_imageMemory = nullptr;
 
    VkFormat getDepthFormat() { return m_format; }
    VkImage& getDepthImage() { return m_image; }
    VkImageView& getDepthImageView() { return m_imageView; }
    VkDeviceMemory& getDepthImageMemory() { return m_imageMemory; }
    
	void init()
	{
		createImage(m_image, m_imageMemory, Swapchain::getSwapchainExtent().width, Swapchain::getSwapchainExtent().height, m_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		createImageView(m_imageView, m_image, m_format, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

    void clear() {
        vkDestroyImageView(ZDevice::getDevice(), m_imageView, nullptr);
        vkDestroyImage(ZDevice::getDevice(), m_image, nullptr);
        vkFreeMemory(ZDevice::getDevice(), m_imageMemory, nullptr);
    }
}
