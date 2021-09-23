#ifndef H_DESC_SET_LAYOUT
#define H_DESC_SET_LAYOUT

#include <vector>
#include <unordered_map>
#include <stdio.h>
#include "Device.h"

namespace ZeroVulkan
{
	struct ZDescriptorInfo
	{
		uint32_t binding;
		union
		{
			VkDescriptorImageInfo* imageInfos;
			VkDescriptorBufferInfo* bufferInfos;
		};
	};

	class ZDescriptorSetLayout
	{
	public:
		ZDescriptorSetLayout() {};
		inline ~ZDescriptorSetLayout();

		inline const std::vector<VkDescriptorSetLayoutBinding>& getBindings() const { return m_bindings; }

		inline void addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);

		inline void createLayout();

		VkDescriptorSetLayout layout = nullptr;

	private:
		std::vector<VkDescriptorSetLayoutBinding> m_bindings;
	};

	ZDescriptorSetLayout::~ZDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(ZDevice::getDevice(), layout, nullptr);
	}

	void ZDescriptorSetLayout::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags)
	{
		VkDescriptorSetLayoutBinding LayoutBinding = {};
		LayoutBinding.binding = binding;
		LayoutBinding.descriptorType = descriptorType;
		LayoutBinding.descriptorCount = 1;
		LayoutBinding.stageFlags = stageFlags;
		LayoutBinding.pImmutableSamplers = nullptr;

		m_bindings.emplace_back(LayoutBinding);
	}

	void ZDescriptorSetLayout::createLayout()
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

#endif // H_DESC_SET_LAYOUT
