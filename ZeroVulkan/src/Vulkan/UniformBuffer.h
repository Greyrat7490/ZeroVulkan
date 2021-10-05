#ifndef H_UNIFORM_BUFFER
#define H_UNIFORM_BUFFER

#include <algorithm>
#include <vector>
#include <memory.h>
#include "Memory.h"
#include "Buffer.h"
#include "Device.h"

namespace ZeroVulkan
{
	class ZUniformLayout
	{
	public:
		template<typename... Layout>
		ZUniformLayout(Layout&&... components);
		
		inline VkDescriptorBufferInfo* getBufferInfo() const { return m_bufferInfo; }
		inline uint32_t getSize() const { return m_uniformSize; }
		inline uint32_t getComponentsCount() const { return m_componentsCount; }

	private:
		VkDeviceSize m_uniformSize = 0;
		uint32_t m_componentsCount = 0;
		VkDescriptorBufferInfo* m_bufferInfo = nullptr;
	};

	template<typename ...Layout>
	inline ZUniformLayout::ZUniformLayout(Layout&& ...components)
	{
		m_componentsCount = sizeof...(components);
		printf("UniformComponents count: %d\n", m_componentsCount);

		auto layoutArray = { components... };

		std::for_each(layoutArray.begin(), layoutArray.end(), [&](auto component) {
			m_uniformSize += static_cast<VkDeviceSize>(component);
		} );

		printf("uniformSize: %ld\n", m_uniformSize);
	}

	class ZUniform
	{
	public:
		ZUniform() = default;
		~ZUniform();

		inline VkDescriptorBufferInfo* getBufferInfo() const { return m_bufferInfo; }
		inline VkDeviceMemory& getMemory() { return m_memory; }
		inline VkDeviceSize getBufferSize() const { return m_uniformSize; }
		inline VkDeviceSize getDynamicAlignment() const { return m_dynamicAlignment; }
	
        // TODO: better update function. Maybe something like a Struct as argument with references to 
		template<typename T, typename... Ts>
		void update(uint32_t offset, T first, Ts&&... ubo);

		template<typename T>
		void update(uint32_t offset, T ubo);

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
		
		void setDynamicAlignments();
	};

	inline void ZUniform::setDynamicAlignments()
	{
		VkPhysicalDeviceProperties devProps;
		vkGetPhysicalDeviceProperties(*ZDevice::getPhysicalDev(), &devProps);

		size_t minAligment = devProps.limits.minUniformBufferOffsetAlignment;

		m_dynamicAlignment = m_layout->getSize();
		m_dynamicAlignment = (m_dynamicAlignment + minAligment - 1) & ~(minAligment - 1);

		printf("uniformDynamicAlignment: %ld\n", m_dynamicAlignment);
		printf("minUniformBufferOffsetAlignment: %zu\n", minAligment);
	}

	inline void ZUniform::create(ZUniformLayout* layout, uint32_t objectCount)
	{
		m_layout = layout;
		
		if(objectCount == 1)
		{
			m_uniformSize = layout->getSize();

			createBuffer(
				ZDevice::getDevice(),
				m_uniformSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				m_buffer,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_memory
			);

			m_bufferInfo = new VkDescriptorBufferInfo;
			m_bufferInfo->offset = 0;
			m_bufferInfo->range = m_uniformSize;
			m_bufferInfo->buffer = m_buffer;
		}
		else
		{
			setDynamicAlignments();

			m_uniformSize = m_dynamicAlignment * objectCount;

			// create dynamic uniformBuffer
			createBuffer(
				ZDevice::getDevice(),
				m_uniformSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				m_buffer,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				m_memory
			);

			m_bufferInfo = new VkDescriptorBufferInfo;
			m_bufferInfo->offset = 0;
			m_bufferInfo->range = m_dynamicAlignment;
			m_bufferInfo->buffer = m_buffer;
		}

		vkMapMemory(ZDevice::getDevice(), m_memory, 0, m_uniformSize, 0, &mappedData);
	}

	inline ZUniform::~ZUniform()
	{
        // vkUnmapMemory(ZDevice::getDevice(), m_memory);
		vkDestroyBuffer(ZDevice::getDevice(), m_buffer, nullptr);
		vkFreeMemory(ZDevice::getDevice(), m_memory, nullptr);

        printf("Destroyed uniformBuffer\n");
	};

	template<typename T, typename... Ts>
	inline void ZUniform::update(uint32_t offset, T first, Ts&&... ubo)
	{
		char* offsetPtr = (char*)mappedData;
		offsetPtr += offset;

		memcpy(offsetPtr, &first, sizeof(T));
		offset += sizeof(first);
		update(offset, ubo ...);
	}

	template<typename T>
	inline void ZUniform::update(uint32_t offset, T ubo)
	{
		char* offsetPtr = (char*)mappedData;
		offsetPtr += offset;

		memcpy(offsetPtr, &ubo, sizeof(T));
	}

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
