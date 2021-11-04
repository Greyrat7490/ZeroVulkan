#include "StorageBuffer.h"

#include "types.h"
#include "Device.h"
#include "Buffer.h"


namespace ZeroVulkan {
    ZStorageBuffer::~ZStorageBuffer() {
        vkDestroyBuffer(ZDevice::getDevice(), m_buffer, nullptr);
        vkFreeMemory(ZDevice::getDevice(), m_memory, nullptr);
    }

    void ZStorageBuffer::set(void* buffer)
    {
        VkBuffer staggingBuffer;
        VkDeviceMemory memory;

        createBuffer(
            ZDevice::getDevice(),
            m_size,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            staggingBuffer,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            memory
        );

        vkMapMemory(ZDevice::getDevice(), memory, 0, m_size, 0, &m_mappedData);
        
        memcpy(m_mappedData, buffer, m_size);

        copyBuffer(staggingBuffer, m_buffer, m_size);

        vkUnmapMemory(ZDevice::getDevice(), memory);
        vkFreeMemory(ZDevice::getDevice(), memory, nullptr);
        vkDestroyBuffer(ZDevice::getDevice(), staggingBuffer, nullptr);
    }

    void ZStorageBuffer::create() {
        createBuffer(
            ZDevice::getDevice(),
            m_size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            m_buffer,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_memory
        );

        m_bufferInfo.offset = 0;
        m_bufferInfo.range = m_size;
        m_bufferInfo.buffer = m_buffer;

        printf("storageSize: %zu\n", m_size);

        if (m_size % 16)
            printf("storageSize is not a multiple of 16 (see glsl alignment requirements)\n");
    }
    
    bool ZStorageBuffer::addComponentByStr(const std::string& str, size_t count) {
        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new else if)");

        if      (str == "mat4")
            m_size += sizeof(mat4) * count;
        else if (str == "vec4")
            m_size += sizeof(vec4) * count;
        else if (str == "vec3")
            m_size += sizeof(vec3) * count;
        else if (str == "vec2")
            m_size += sizeof(vec2) * count;
        else if (str == "float")
            m_size += sizeof(float) * count;
        else
            return false;

        return true;
    }
}
