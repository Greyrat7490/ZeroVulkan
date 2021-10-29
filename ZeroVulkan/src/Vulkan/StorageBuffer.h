#ifndef ZSTOREAGEBUFFER_H_
#define ZSTOREAGEBUFFER_H_

#include "vulkan/vulkan_core.h"
#include <string>

namespace ZeroVulkan {
	class ZStorageBuffer
	{
    public:
        ~ZStorageBuffer();
        
    inline const VkDescriptorBufferInfo* getBufferInfo() { return &m_bufferInfo; }
    inline const VkBuffer* getBuffer() { return &m_buffer; }

	void set(void* buffer);
    bool addComponentByStr(const std::string& str, size_t count = 1);
    void create();
    
    private:        
		void* m_mappedData = nullptr;
		VkBuffer m_buffer = nullptr;
		VkDeviceMemory m_memory = nullptr;
		size_t m_size = 0;
		VkDescriptorBufferInfo m_bufferInfo;
	};
}

#endif // ZSTOREAGEBUFFER_H_
