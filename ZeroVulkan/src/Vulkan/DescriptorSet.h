#ifndef H_DESC_SET
#define H_DESC_SET

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
        std::vector<VkWriteDescriptorSet> writeDescSets;
	};
}

#endif // H_DESC_SET
