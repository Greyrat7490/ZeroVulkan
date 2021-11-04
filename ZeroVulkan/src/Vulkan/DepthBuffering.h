#ifndef H_DEPTH_BUFFERING
#define H_DEPTH_BUFFERING

#include "Device.h"

namespace ZeroVulkan::DepthBuffering {
    void init();
    void clear();

    VkFormat getDepthFormat();
    VkImage& getDepthImage();
    VkImageView& getDepthImageView();
    VkDeviceMemory& getDepthImageMemory();
}

#endif // H_DEPTH_BUFFERING
