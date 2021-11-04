#ifndef DESC_SET_LAYOUT_H_
#define DESC_SET_LAYOUT_H_

#include <vector>
#include <stdio.h>
#include "Device.h"

namespace ZeroVulkan
{
    union ZDescriptorInfo
    {
        const VkDescriptorImageInfo* imageInfo;
        const VkDescriptorBufferInfo* bufferInfo;
    };

    class ZDescriptorSetLayout
    {
    public:
        inline ~ZDescriptorSetLayout();

        inline const std::vector<VkDescriptorSetLayoutBinding>& getBindings() const { return m_bindings; }
        inline const std::vector<ZDescriptorInfo>& getDescInfos() const { return m_descInfos; }

        inline void addBinding(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
        inline void addBinding(uint32_t binding, const VkDescriptorImageInfo* imageInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);

        inline void create();

        VkDescriptorSetLayout layout = nullptr;

    private:
        std::vector<VkDescriptorSetLayoutBinding> m_bindings;
        std::vector<ZDescriptorInfo> m_descInfos;
    };

    ZDescriptorSetLayout::~ZDescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(ZDevice::getDevice(), layout, nullptr);
    }

    void ZDescriptorSetLayout::addBinding(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags)
    {
        VkDescriptorSetLayoutBinding layoutBinding = {};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = stageFlags;
        layoutBinding.pImmutableSamplers = nullptr;

        m_bindings.push_back(layoutBinding);

        ZDescriptorInfo info;
        info.bufferInfo = bufferInfo;
        m_descInfos.push_back(info);
    }

    void ZDescriptorSetLayout::addBinding(uint32_t binding, const VkDescriptorImageInfo* imageInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags)
    {
        VkDescriptorSetLayoutBinding layoutBinding = {};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = stageFlags;
        layoutBinding.pImmutableSamplers = nullptr;

        m_bindings.push_back(layoutBinding);

        ZDescriptorInfo info;
        info.imageInfo = imageInfo;
        m_descInfos.push_back(info);
    }
    
    void ZDescriptorSetLayout::create()
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = m_bindings.size();
        layoutInfo.pBindings = m_bindings.data();

        VkResult res = vkCreateDescriptorSetLayout(ZDevice::getDevice(), &layoutInfo, nullptr, &layout);

        if (res != VK_SUCCESS)
            printf("create desc layput ERROR: %d\n", res);
    }
}

#endif // DESC_SET_LAYOUT_H_
