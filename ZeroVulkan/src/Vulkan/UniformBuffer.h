#ifndef H_UNIFORM_BUFFER
#define H_UNIFORM_BUFFER

#include <algorithm>
#include <utility>
#include <vector>
#include <memory.h>
#include <any>
#include "Memory.h"
#include "Buffer.h"
#include "Device.h"
#include "types.h"

namespace ZeroVulkan
{
	class ZUniformLayout
	{
	public:
        ZUniformLayout() = default;
        ZUniformLayout(ZUniformLayout&& source);
        ZUniformLayout& operator=(ZUniformLayout&& source);
        
	    std::any* addComponent(ZType type);
        
        inline VkDescriptorBufferInfo* getBufferInfo() const { return m_bufferInfo; }
		inline uint32_t getSize() const { return m_uniformSize; }
		inline uint32_t getComponentsCount() const { return m_components.size(); }

	private:
        friend class ZUniform;
        
		VkDeviceSize m_uniformSize = 0;
        std::vector<std::pair<ZType, std::any>> m_components;
		VkDescriptorBufferInfo* m_bufferInfo = nullptr;
	};

    

	class ZUniform
	{
	public:
		ZUniform() = default;
        ZUniform(ZUniform&& source);
        ZUniform& operator=(ZUniform&& source);
		~ZUniform();

		inline VkDescriptorBufferInfo* getBufferInfo() const { return m_bufferInfo; }
		inline VkDeviceMemory& getMemory() { return m_memory; }
		inline VkDeviceSize getBufferSize() const { return m_uniformSize; }
		inline VkDeviceSize getDynamicAlignment() const { return m_dynamicAlignment; }
	
		void update();

		template<typename T>
		void dynamicUpdate(T* ubos, uint32_t objectCount);

		void create(ZUniformLayout* layout, uint32_t objectCount = 1);
		void* mappedData = nullptr;
	private:
		ZUniformLayout* m_layout = nullptr;
		VkBuffer m_buffer = nullptr;
		VkDeviceMemory m_memory = nullptr;
		VkDeviceSize m_uniformSize = 0;
		VkDescriptorBufferInfo* m_bufferInfo = nullptr;
		VkDeviceSize m_dynamicAlignment = 0;
		
        bool ready = false;
        
		void setDynamicAlignments();
	};

	template<typename T>//TODO: only update parts which actually changed
	inline void ZUniform::dynamicUpdate(T* ubos, uint32_t objectCount)
	{
		VkMappedMemoryRange memoryRange = {};
		memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		memoryRange.offset = 0;
		memoryRange.size = sizeof(T) * objectCount;
		memoryRange.memory = m_memory;
		vkFlushMappedMemoryRanges(ZDevice::getDevice(), 1, &memoryRange);

		char* mappedDataArray = (char*)mappedData;

		for(uint32_t i = 0; i < objectCount; i++)
			memcpy(&mappedDataArray[i * m_dynamicAlignment], &ubos[i], sizeof(T));
	}
}

#endif // H_UNIFORM_BUFFER
