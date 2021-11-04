#ifndef DESC_POOL_H_
#define DESC_POOL_H_

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

#endif // DESC_POOL_H_
