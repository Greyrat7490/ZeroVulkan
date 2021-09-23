#ifndef H_SWAPCHAIN
#define H_SWAPCHAIN

#include "Device.h"
#include "Image.h"
#include "Surface.h"
#include "RenderPass.h"
#include "SyncObjects.h"
#include "CommandBuffer.h"

namespace ZeroVulkan
{
    inline void chooseSwapchainPresentMode()
    {
        std::vector<VkPresentModeKHR> availablePresentModes;

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &presentModeCount, nullptr);

        if (presentModeCount)
        {
            availablePresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::getPhysicalDev()[0], ZDevice::getSurface(), &presentModeCount, availablePresentModes.data());
        }

        ZDevice::getSwapchainPresentMode() = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                ZDevice::getSwapchainPresentMode() = availablePresentMode;
        }
    }

    inline void createSwapchainImgViews()
    {
        ZDevice::getSwapchainImageViews().resize(ZDevice::getSwapchainImages().size());

        for (uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
            createImageView(ZDevice::getSwapchainImageViews()[i], ZDevice::getSwapchainImages()[i], ZDevice::getSwapchainImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
    }

    inline void createSwapchain(VkExtent2D extent)
    {
        chooseSwapchainPresentMode();
        setSurfaceExtent(extent.width, extent.height);

        uint32_t imageCount = ZDevice::getSurfaceCapabilities().maxImageCount;

        //double/trippleBuffering -> (can) prevent screentearing
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = ZDevice::getSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = ZDevice::getSurfaceFormat().format;
        createInfo.imageColorSpace = ZDevice::getSurfaceFormat().colorSpace;
        createInfo.imageExtent = ZDevice::getSwapchainExtent();
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 0; // TODO: change and what does it change exactly
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.preTransform = ZDevice::getSurfaceCapabilities().currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = ZDevice::getSwapchainPresentMode();
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = ZDevice::getSwapchain();

        printf("swapchainPresentMode: %d\n", (int)ZDevice::getSwapchainPresentMode());

        VkResult res = vkCreateSwapchainKHR(ZDevice::getDevice(), &createInfo, nullptr, &ZDevice::getSwapchain());
        if (res != VK_SUCCESS)
            printf("create swapchain ERROR: %d\n", res);

        vkGetSwapchainImagesKHR(ZDevice::getDevice(), ZDevice::getSwapchain(), &imageCount, nullptr);
        ZDevice::getSwapchainImages().resize(imageCount);
        vkGetSwapchainImagesKHR(ZDevice::getDevice(), ZDevice::getSwapchain(), &imageCount, ZDevice::getSwapchainImages().data());

        ZDevice::getSwapchainImageFormat() = ZDevice::getSurfaceFormat().format;
    }

    inline void createSwapchain(uint32_t width, uint32_t height)
    {
        VkExtent2D extent = { width, height };
        createSwapchain(extent);
    }

    inline void createFramebuffers(uint32_t width, uint32_t height)
    {
        ZDevice::getSwapchainFramebuffers().resize( ZDevice::getSwapchainImageViews().size());
        
        for (uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
        {
            VkImageView attachments[] = { ZDevice::getSwapchainImageViews()[i], ZDevice::getDepthImageView() };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = ZDevice::getRenderPass();
            framebufferCreateInfo.attachmentCount = 2;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = width;
            framebufferCreateInfo.height = height;
            framebufferCreateInfo.layers = 1;

            VkResult res = vkCreateFramebuffer(ZDevice::getDevice(), &framebufferCreateInfo, nullptr, &ZDevice::getSwapchainFramebuffers()[i]);
            if (res != VK_SUCCESS)
                printf("create framebuffer ERROR: %d\n", res);
        }
    }
}

#endif // H_SWAPCHAIN
