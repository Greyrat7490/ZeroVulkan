#include "Swapchain.h"

#include "Image.h"
#include "Device.h"
#include "Surface.h"
#include "RenderPass.h"
#include "DepthBuffering.h"

#include "Window/window.h"


namespace ZeroVulkan::Swapchain {
    static VkSwapchainKHR s_swapchain = nullptr;
    static std::vector<VkImage> s_images;
    static std::vector<VkImageView> s_imageViews;

    static VkPresentModeKHR s_presentMode;
    static VkFormat s_imageFormat;
    static VkExtent2D s_extent;
    static std::vector<VkFramebuffer> s_framebuffers;

    static uint32_t s_curFrame = 0;
    

    const std::vector<VkImage>& getSwapchainImages() { return s_images; }
    const std::vector<VkImageView>& getSwapchainImageViews() { return s_imageViews; }
    const std::vector<VkFramebuffer>& getSwapchainFramebuffers() { return s_framebuffers; }

    VkSwapchainKHR* getSwapchain() { return &s_swapchain; }
    VkPresentModeKHR getPresentMode() { return s_presentMode; }
    VkFormat getImageFormat() { return s_imageFormat; }
    VkExtent2D getExtent() { return s_extent; }

    uint32_t getCurFrame() { return s_curFrame; }

    
    void chooseSwapchainPresentMode()
    {
        std::vector<VkPresentModeKHR> availablePresentModes;

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::getPhysicalDev()[0], Surface::getSurface(), &presentModeCount, nullptr);

        if (presentModeCount)
        {
            availablePresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::getPhysicalDev()[0], Surface::getSurface(), &presentModeCount, availablePresentModes.data());
        }

#ifdef Z_DEBUG
        printf("availablePresentModes:");
        for (const VkPresentModeKHR availablePresentMode : availablePresentModes)
            printf(" %d", availablePresentMode);
        
        puts("");
#endif

        // VK_PRESENT_MODE_FIFO_KHR(VSync) causes stuttering on the whole system, which seems to be a NVidia driver bug
        // same happens with vkcube and VK_PRESENT_MODE_FIFO_KHR
        // https://forums.developer.nvidia.com/t/hangs-freezes-when-vulkan-v-sync-vk-present-mode-fifo-khr-is-enabled/67751/15
        // https://www.reddit.com/r/vulkan/comments/gblf3g/catastrophic_desktop_performance_issues_with/
        s_presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

        for (const VkPresentModeKHR availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                s_presentMode = availablePresentMode;
        }
    }

    void createSwapchainImgViews()
    {
        s_imageViews.resize(s_images.size());

        for (uint32_t i = 0; i < s_imageViews.size(); i++)
            createImageView(s_imageViews[i], s_images[i], s_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void create(VkExtent2D extent)
    {
        chooseSwapchainPresentMode();
        s_extent = Surface::getCapableExtent(extent);

        uint32_t imageCount = Surface::getSurfaceCapabilities().maxImageCount;

        //double/trippleBuffering -> (can) prevent screentearing
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Surface::getSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = Surface::getSurfaceFormat().format;
        createInfo.imageColorSpace = Surface::getSurfaceFormat().colorSpace;
        createInfo.imageExtent = s_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 0; // TODO: change and what does it change exactly
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.preTransform = Surface::getSurfaceCapabilities().currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = s_presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = s_swapchain;

        printf("swapchainPresentMode: %d\n", (int)s_presentMode);

        VkResult res = vkCreateSwapchainKHR(ZDevice::getDevice(), &createInfo, nullptr, &s_swapchain);
        if (res != VK_SUCCESS)
            printf("create swapchain ERROR: %d\n", res);

        vkGetSwapchainImagesKHR(ZDevice::getDevice(), s_swapchain, &imageCount, nullptr);
        s_images.resize(imageCount);
        vkGetSwapchainImagesKHR(ZDevice::getDevice(), s_swapchain, &imageCount, s_images.data());

        s_imageFormat = Surface::getSurfaceFormat().format;


        createSwapchainImgViews();
        RenderPass::create(s_imageFormat);
        DepthBuffering::init();
        createFramebuffers(s_extent.width, s_extent.height);
    }

    void create(uint32_t width, uint32_t height)
    {
        VkExtent2D extent = { width, height };
        create(extent);
    }

    void createFramebuffers(uint32_t width, uint32_t height)
    {
        s_framebuffers.resize( s_imageViews.size());
        
        for (uint32_t i = 0; i < s_imageViews.size(); i++)
        {
            VkImageView attachments[] = { s_imageViews[i], DepthBuffering::getDepthImageView() };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = *RenderPass::getRenderPass();
            framebufferCreateInfo.attachmentCount = 2;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = width;
            framebufferCreateInfo.height = height;
            framebufferCreateInfo.layers = 1;

            VkResult res = vkCreateFramebuffer(ZDevice::getDevice(), &framebufferCreateInfo, nullptr, &s_framebuffers[i]);
            if (res != VK_SUCCESS)
                printf("create framebuffer ERROR: %d\n", res);
        }
    }

    void clear() {
        DepthBuffering::clear();
        RenderPass::clear();
        
        for (uint32_t i = 0; i < s_framebuffers.size(); i++)
            vkDestroyFramebuffer(ZDevice::getDevice(), s_framebuffers[i], nullptr);

        for (uint32_t i = 0; i < s_imageViews.size(); i++)
            vkDestroyImageView(ZDevice::getDevice(), s_imageViews[i], nullptr);

        vkDestroySwapchainKHR(ZDevice::getDevice(), s_swapchain, nullptr);
    }

    void nextFrame() {
        s_curFrame = (s_curFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void refresh() {
        vec2 winSize = ZWindow::getSize();
        VkExtent2D extent = { (uint32_t) winSize[0], (uint32_t) winSize[1] };
        extent = Surface::getCapableExtent(extent);
        
        DepthBuffering::clear();
        RenderPass::clear();
        
        for (uint32_t i = 0; i < s_framebuffers.size(); i++)
            vkDestroyFramebuffer(ZDevice::getDevice(), s_framebuffers[i], nullptr);

        for (uint32_t i = 0; i < s_imageViews.size(); i++)
            vkDestroyImageView(ZDevice::getDevice(), s_imageViews[i], nullptr);

        VkSwapchainKHR old = s_swapchain;
        
        create(extent.width, extent.height);
        
        vkDestroySwapchainKHR(ZDevice::getDevice(), old, nullptr);
    }
}
