#ifndef H_DESC_POOL
#define H_DESC_POOL

#include <vector>
#include "Device.h"
#include "DescriptorSet.h"

namespace ZeroVulkan
{
    class ZDescriptorPool
    {
    public:
        ZDescriptorPool() = default;
        ~ZDescriptorPool();

        VkDescriptorPool descriptorPool = nullptr;
        uint32_t maxSets = 1;
        
        void create();
        void addDescriptorLayout(const ZDescriptorSetLayout* descLayout);
    private:
        std::vector<VkDescriptorPoolSize> poolSizes;
    };
}

#endif // H_DESC_POOL
