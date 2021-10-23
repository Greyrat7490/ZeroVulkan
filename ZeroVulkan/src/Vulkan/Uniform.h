#ifndef H_UNIFORM_BUFFER
#define H_UNIFORM_BUFFER

#include <algorithm>
#include <typeinfo>
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
	class ZUniform
	{
	public:
		ZUniform() = default;
        ZUniform(ZUniform&& source);
        ZUniform& operator=(ZUniform&& source);
		~ZUniform();

        inline const VkDescriptorBufferInfo* getBufferInfo() { return &m_bufferInfo; }
		inline VkDeviceMemory getMemory() const { return m_memory; }
		inline VkDeviceSize getBufferSize() const { return m_size; }
		inline VkDeviceSize getDynamicAlignment() const { return m_dynamicAlignment; }
	
		void update();

	    void addComponent(ZType type);
        
        template<typename T>
        T* getComponent(size_t index);

		template<typename T>
		void dynamicUpdate(T* ubos, uint32_t objectCount);

		void create(uint32_t objectCount = 1);
	private:
		void* m_mappedData = nullptr;
		VkBuffer m_buffer = nullptr;
		VkDeviceMemory m_memory = nullptr;
		VkDeviceSize m_size = 0;
		VkDescriptorBufferInfo m_bufferInfo;
		VkDeviceSize m_dynamicAlignment = 0;
        
        std::vector<std::pair<size_t, size_t>> m_components;
		
        bool ready = false;
        
		void setDynamicAlignments();
	};

    
    template<typename T>
    inline T* ZUniform::getComponent(size_t index) {
        if (index >= m_components.size()) {
            printf("ERROR: uniform has only %zu components", m_components.size());
            return nullptr;
        }
            
        // TODO: printf human readable types
        if (typeid(T).hash_code() != m_components[index].first) {
            printf("ERROR: couldn't get uniform component at index %zu\n"
                    "\t the type you passed is different to the actual type\n", index);
            return nullptr;
        }

        return (T*)((char*)m_mappedData + m_components[index].second);
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

		char* mappedDataArray = (char*)m_mappedData;

		for(uint32_t i = 0; i < objectCount; i++)
			memcpy(&mappedDataArray[i * m_dynamicAlignment], &ubos[i], sizeof(T));
	}
}

#endif // H_UNIFORM_BUFFER
