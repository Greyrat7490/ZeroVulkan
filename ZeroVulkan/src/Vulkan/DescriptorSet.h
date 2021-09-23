#ifndef H_DESC_SET
#define H_DESC_SET

#include <unordered_map>
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

namespace ZeroVulkan
{
	class ZDescriptorSet
	{
	public:
		ZDescriptorSet() {};
		ZDescriptorSet(ZDescriptorSetLayout* descLayout) 
			: m_descLayout( descLayout ) {};
		inline ~ZDescriptorSet() { m_descriptorInfos.clear(); };

		VkDescriptorSet descSet = nullptr;
		
		void addDescriptorInfo(uint32_t binding, VkDescriptorBufferInfo* bufferinfo);
		void addDescriptorInfo(uint32_t binding, VkDescriptorImageInfo* imageinfo);

		inline void setLayout(ZDescriptorSetLayout* descLayout) { m_descLayout = descLayout; };

		void create(VkDescriptorPool descriptorPool);
	private:
		ZDescriptorSetLayout* m_descLayout = nullptr;
		std::unordered_map<uint32_t, ZDescriptorInfo> m_descriptorInfos;
	};

	inline void ZDescriptorSet::addDescriptorInfo(uint32_t binding, VkDescriptorBufferInfo* bufferinfo)
	{
		m_descriptorInfos[binding].bufferInfos = bufferinfo;
	}

	inline void ZDescriptorSet::addDescriptorInfo(uint32_t binding, VkDescriptorImageInfo* imageinfo)
	{
		m_descriptorInfos[binding].imageInfos = imageinfo;
	}

	inline void ZDescriptorSet::create(VkDescriptorPool descriptorPool)
	{
		auto& bindings = m_descLayout->getBindings();

		if(!bindings.empty())
		{
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.pNext = nullptr;
			descriptorSetAllocateInfo.descriptorPool = descriptorPool;
			descriptorSetAllocateInfo.descriptorSetCount = 1;
			descriptorSetAllocateInfo.pSetLayouts = &m_descLayout->layout;

			VkResult res = vkAllocateDescriptorSets(ZDevice::getDevice(), &descriptorSetAllocateInfo, &descSet);
            if (res != VK_SUCCESS)
                printf("Allocate desc-set ERROR: %d\n", res);

			VkWriteDescriptorSet* writeDescriptorSets = new VkWriteDescriptorSet[bindings.size()];

			for (uint32_t i = 0; i < bindings.size(); i++)
			{
				writeDescriptorSets[i] = {};
				writeDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDescriptorSets[i].dstSet = descSet;
				writeDescriptorSets[i].dstBinding = bindings[i].binding;
				writeDescriptorSets[i].dstArrayElement = 0;
				writeDescriptorSets[i].descriptorCount = 1;
				writeDescriptorSets[i].descriptorType = bindings[i].descriptorType;

				switch(bindings[i].descriptorType)
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					writeDescriptorSets[i].pBufferInfo = m_descriptorInfos[bindings[i].binding].bufferInfos;
					break;
				case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					writeDescriptorSets[i].pImageInfo = m_descriptorInfos[bindings[i].binding].imageInfos;
					break;
				default:
					printf("ERROR: DescriptorTyp is still not supported\n");
					break;
				}
				// pTexelBufferView:  watch later
			}

			vkUpdateDescriptorSets(ZDevice::getDevice(), bindings.size(), writeDescriptorSets, 0, nullptr);
		}
		else
			printf("no bindings defined so there is no need to create the DescriptorSet\n");
	}
}

#endif // H_DESC_SET
