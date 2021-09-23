#ifndef H_DESC_POOL
#define H_DESC_POOL

#include <vector>
#include "Device.h"
#include "UniformBuffer.h"
#include "DescriptorSetLayout.h"

namespace ZeroVulkan
{
    class ZDescriptorPool
    {
    public:
        ZDescriptorPool() {};
        ~ZDescriptorPool();

        VkDescriptorPool descriptorPool = nullptr;
        
        void create(uint32_t descCount = 1);
        void addDescriptorLayout(const ZDescriptorSetLayout* descLayout, uint32_t descCount = 1);
    private:
        std::vector<VkDescriptorPoolSize> poolSizes;
    };

    inline void ZDescriptorPool::create(uint32_t descCount)
    {   
        if(!poolSizes.empty())
        {
            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.poolSizeCount = poolSizes.size();
            poolInfo.maxSets = descCount;//watch later

            VkResult res = vkCreateDescriptorPool(ZDevice::getDevice(), &poolInfo, nullptr, &descriptorPool);
            if (res != VK_SUCCESS)
                printf("create desc pool ERROR %d\n", res);
        }
        else
            printf("poolSizes was empty so Descriptor is not necessary\n");
    }

    inline ZDescriptorPool::~ZDescriptorPool()
    {
        vkDestroyDescriptorPool(ZDevice::getDevice(), descriptorPool, nullptr);
    }

    inline void ZDescriptorPool::addDescriptorLayout(const ZDescriptorSetLayout* descLayout, uint32_t descCount)
    {
        uint32_t offset = poolSizes.size();
        poolSizes.resize(poolSizes.size() + descLayout->getBindings().size());

        for (uint32_t i = 0; i < descLayout->getBindings().size(); i++)
        {
            poolSizes[i + offset].type = descLayout->getBindings()[i].descriptorType;
            poolSizes[i + offset].descriptorCount = descCount;

            printf("poolSize.type %d\n", (int)poolSizes[i + offset].type);
            printf("poolSize.descriptorCount %d\n", poolSizes[i + offset].descriptorCount);
        }
    }
}

#endif // H_DESC_POOL
