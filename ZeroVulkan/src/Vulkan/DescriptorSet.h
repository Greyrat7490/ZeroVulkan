#ifndef DESC_SET_H_
#define DESC_SET_H_

#include <vector>
#include "DescriptorSetLayout.h"
#include "utils.h"

namespace ZeroVulkan
{
    class ZDescriptorSet
    {
    public:
        ZDescriptorSet() = default;

        VkDescriptorSet descSet = nullptr;

        void create(const ZDescriptorSetLayout* descLayout, VkDescriptorPool descriptorPool);
    private:
        const ZDescriptorSetLayout* m_descLayout = nullptr;
        std::vector<VkWriteDescriptorSet> m_writeDescSets;
    };
}

#endif // DESC_SET_H_
