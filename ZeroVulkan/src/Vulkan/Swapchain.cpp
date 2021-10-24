#include "Swapchain.h"
#include "Surface.h"
#include "Device.h"
#include "Image.h"
#include "RenderPass.h"
#include "DepthBuffering.h"
#include "Window/window.h"

namespace ZeroVulkan::Swapchain {
    VkSwapchainKHR m_swapchain = nullptr;
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;

    VkPresentModeKHR m_presentMode;
    VkFormat m_imageFormat;
    VkExtent2D m_extent;
    std::vector<VkFramebuffer> m_framebuffers;;

    VkRenderPass m_renderPass = nullptr;
    
    uint32_t curFrame = 0;
    
    
    const VkSwapchainKHR& getSwapchain() { return m_swapchain; }
    const std::vector<VkImage>& getSwapchainImages() { return m_images; }
    const std::vector<VkImageView>& getSwapchainImageViews() { return m_imageViews; }

    const VkPresentModeKHR& getSwapchainPresentMode() { return m_presentMode; }
    const VkFormat& getSwapchainImageFormat() { return m_imageFormat; }
    const VkExtent2D& getSwapchainExtent() { return m_extent; }
    const std::vector<VkFramebuffer>& getSwapchainFramebuffers() { return m_framebuffers; }

    uint32_t& getCurFrame() { return curFrame; }

    
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
        
        printf("\n");
#endif

        // VK_PRESENT_MODE_FIFO_KHR(VSync) causes stuttering on the whole system, which seems to be a NVidia driver bug
        // same happens with vkcube and VK_PRESENT_MODE_FIFO_KHR
        // https://forums.developer.nvidia.com/t/hangs-freezes-when-vulkan-v-sync-vk-present-mode-fifo-khr-is-enabled/67751/15
        // https://www.reddit.com/r/vulkan/comments/gblf3g/catastrophic_desktop_performance_issues_with/
        m_presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

        for (const VkPresentModeKHR availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                m_presentMode = availablePresentMode;
        }
    }

    void createSwapchainImgViews()
    {
        m_imageViews.resize(m_images.size());

        for (uint32_t i = 0; i < m_imageViews.size(); i++)
            createImageView(m_imageViews[i], m_images[i], m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void create(VkExtent2D extent)
    {
        chooseSwapchainPresentMode();
        m_extent = Surface::getCapableExtent(extent);

        uint32_t imageCount = Surface::getSurfaceCapabilities().maxImageCount;

        //double/trippleBuffering -> (can) prevent screentearing
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Surface::getSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = Surface::getSurfaceFormat().format;
        createInfo.imageColorSpace = Surface::getSurfaceFormat().colorSpace;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 0; // TODO: change and what does it change exactly
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.preTransform = Surface::getSurfaceCapabilities().currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = m_swapchain;

        printf("swapchainPresentMode: %d\n", (int)m_presentMode);

        VkResult res = vkCreateSwapchainKHR(ZDevice::getDevice(), &createInfo, nullptr, &m_swapchain);
        if (res != VK_SUCCESS)
            printf("create swapchain ERROR: %d\n", res);

        vkGetSwapchainImagesKHR(ZDevice::getDevice(), m_swapchain, &imageCount, nullptr);
        m_images.resize(imageCount);
        vkGetSwapchainImagesKHR(ZDevice::getDevice(), m_swapchain, &imageCount, m_images.data());

        m_imageFormat = Surface::getSurfaceFormat().format;


        createSwapchainImgViews();
        RenderPass::create(m_imageFormat);
        DepthBuffering::init();
        createFramebuffers(m_extent.width, m_extent.height);
    }

    void create(uint32_t width, uint32_t height)
    {
        VkExtent2D extent = { width, height };
        create(extent);
    }

    void createFramebuffers(uint32_t width, uint32_t height)
    {
        m_framebuffers.resize( m_imageViews.size());
        
        for (uint32_t i = 0; i < m_imageViews.size(); i++)
        {
            VkImageView attachments[] = { m_imageViews[i], DepthBuffering::getDepthImageView() };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = *RenderPass::getRenderPass();
            framebufferCreateInfo.attachmentCount = 2;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = width;
            framebufferCreateInfo.height = height;
            framebufferCreateInfo.layers = 1;

            VkResult res = vkCreateFramebuffer(ZDevice::getDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]);
            if (res != VK_SUCCESS)
                printf("create framebuffer ERROR: %d\n", res);
        }
    }

    void clear() {
        DepthBuffering::clear();
        RenderPass::clear();
        
		for (uint32_t i = 0; i < m_framebuffers.size(); i++ )
            vkDestroyFramebuffer(ZDevice::getDevice(), m_framebuffers[i], nullptr);

		for (uint32_t i = 0; i < m_imageViews.size(); i++)
			vkDestroyImageView(ZDevice::getDevice(), m_imageViews[i], nullptr);

		vkDestroySwapchainKHR(ZDevice::getDevice(), m_swapchain, nullptr);
    }

    void nextFrame() {
        curFrame = (curFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void refresh() {
        vec2 winSize = ZWindow::getSize();
        VkExtent2D extent = { (uint32_t) winSize[0], (uint32_t) winSize[1] };
        m_extent = Surface::getCapableExtent(extent);
        
        DepthBuffering::clear();
        RenderPass::clear();
        
		for (uint32_t i = 0; i < m_framebuffers.size(); i++ )
            vkDestroyFramebuffer(ZDevice::getDevice(), m_framebuffers[i], nullptr);

		for (uint32_t i = 0; i < m_imageViews.size(); i++)
			vkDestroyImageView(ZDevice::getDevice(), m_imageViews[i], nullptr);

        VkSwapchainKHR old = m_swapchain;
        
        create(m_extent.width, m_extent.height);
        
		vkDestroySwapchainKHR(ZDevice::getDevice(), old, nullptr);
    }
}
