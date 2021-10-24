#ifndef H_SWAPCHAIN
#define H_SWAPCHAIN

#include "Device.h"

namespace ZeroVulkan::Swapchain {
    static const int MAX_FRAMES_IN_FLIGHT = 2;
    
    const VkSwapchainKHR& getSwapchain();
    const std::vector<VkImage>& getSwapchainImages();
    const std::vector<VkImageView>& getSwapchainImageViews();

    const VkPresentModeKHR& getSwapchainPresentMode();
    const VkFormat& getSwapchainImageFormat();
    const VkExtent2D& getSwapchainExtent();
    const std::vector<VkFramebuffer>& getSwapchainFramebuffers();

    uint32_t& getCurFrame();
    
    void nextFrame();

    void chooseSwapchainPresentMode();
    void createSwapchainImgViews();
    void create(VkExtent2D extent);
    void create(uint32_t width, uint32_t height);
    void createFramebuffers(uint32_t width, uint32_t height);
    void clear();
    void refresh();
}

#endif // H_SWAPCHAIN
