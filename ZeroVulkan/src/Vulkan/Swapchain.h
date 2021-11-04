#ifndef SWAPCHAIN_H_
#define SWAPCHAIN_H_

#include "Device.h"

namespace ZeroVulkan::Swapchain {
    static const int MAX_FRAMES_IN_FLIGHT = 2;
    
    const std::vector<VkImage>& getSwapchainImages();
    const std::vector<VkImageView>& getSwapchainImageViews();
    const std::vector<VkFramebuffer>& getSwapchainFramebuffers();

    VkSwapchainKHR* getSwapchain();
    VkPresentModeKHR getPresentMode();
    VkFormat getImageFormat();
    VkExtent2D getExtent();

    uint32_t getCurFrame();
    void nextFrame();


    void create(VkExtent2D extent);
    void create(uint32_t width, uint32_t height);
    void clear();
    void refresh();
    
    void createFramebuffers(uint32_t width, uint32_t height);
    void chooseSwapchainPresentMode();
    void createSwapchainImgViews();
}

#endif // SWAPCHAIN_H_
