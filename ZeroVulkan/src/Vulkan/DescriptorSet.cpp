#include "DescriptorSet.h"
#include "DescriptorPool.h"

namespace ZeroVulkan {
	void ZDescriptorSet::create(const ZDescriptorSetLayout* descLayout, VkDescriptorPool descriptorPool)
	{
        m_descLayout = descLayout; 
		auto& bindings = m_descLayout->getBindings();
		auto& infos = m_descLayout->getDescInfos();

		if(!bindings.empty())
		{
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.descriptorPool = descriptorPool;
			descriptorSetAllocateInfo.descriptorSetCount = 1;
			descriptorSetAllocateInfo.pSetLayouts = &m_descLayout->layout;

			VkResult res = vkAllocateDescriptorSets(ZDevice::getDevice(), &descriptorSetAllocateInfo, &descSet);
            if (res != VK_SUCCESS)
                printf("Allocate desc-set ERROR: %d\n", res);

            writeDescSets.resize(bindings.size());

			for (uint32_t i = 0; i < bindings.size(); i++)
			{
				writeDescSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDescSets[i].dstSet = descSet;
				writeDescSets[i].dstBinding = bindings[i].binding;
				writeDescSets[i].dstArrayElement = 0;
				writeDescSets[i].descriptorCount = 1;
				writeDescSets[i].descriptorType = bindings[i].descriptorType;

				switch(bindings[i].descriptorType)
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					writeDescSets[i].pBufferInfo = infos[i].bufferInfo;
					break;
				case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					writeDescSets[i].pImageInfo = infos[i].imageInfo;
					break;
				default:
					printf("ERROR: DescriptorTyp is still not supported\n");
					break;
				}
				// pTexelBufferView:  watch later
			}

			vkUpdateDescriptorSets(ZDevice::getDevice(), writeDescSets.size(), writeDescSets.data(), 0, nullptr);
		}
		else
			printf("no bindings defined so there is no need to create the DescriptorSet\n");
	}
}

