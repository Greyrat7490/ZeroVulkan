#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

namespace ZeroVulkan {
    void ZDescriptorPool::create()
    {
        if(!poolSizes.empty())
        {
            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.poolSizeCount = poolSizes.size();
            poolInfo.maxSets = maxSets;//watch later

            VkResult res = vkCreateDescriptorPool(ZDevice::getDevice(), &poolInfo, nullptr, &descriptorPool);
            if (res != VK_SUCCESS)
                printf("create desc pool ERROR %d\n", res);
        }
        else
            printf("poolSizes was empty so Descriptor is not necessary\n");
    }

    ZDescriptorPool::~ZDescriptorPool()
    {
        vkDestroyDescriptorPool(ZDevice::getDevice(), descriptorPool, nullptr);
    }

    void ZDescriptorPool::addDescriptorLayout(const ZDescriptorSetLayout* descLayout)
    {
        for (uint32_t i = 0; i < descLayout->getBindings().size(); i++)
        {
            poolSizes.emplace_back();
            poolSizes.back().type = descLayout->getBindings()[i].descriptorType;
            poolSizes.back().descriptorCount = 1;
        }
    }
}
