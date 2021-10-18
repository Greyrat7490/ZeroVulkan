#include "Uniform.h"
#include <any>

namespace ZeroVulkan {
    void ZUniform::addComponent(ZType type) {
        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new cases)");
        
        switch (type)
        {
        case ZType::MAT4:
            m_components.push_back(std::make_pair(typeid(mat4).hash_code(), m_size));
            m_size += sizeof(mat4);
            break;
        case ZType::VEC4:
            m_components.push_back(std::make_pair(typeid(vec4).hash_code(), m_size));
            m_size += sizeof(vec4);
            break;
        case ZType::VEC3:
            m_components.push_back(std::make_pair(typeid(vec3).hash_code(), m_size));
            m_size += sizeof(vec3);
            break;
        case ZType::VEC2:
            m_components.push_back(std::make_pair(typeid(vec2).hash_code(), m_size));
            m_size += sizeof(vec2);
            break;
        case ZType::FLOAT:
            m_components.push_back(std::make_pair(typeid(float).hash_code(), m_size));
            m_size += sizeof(float);
            break;
        default:
            printf("unknown ZType %d\n", (int)type);
        }
    }

    ZUniform::ZUniform(ZUniform&& source) {
		m_mappedData = source.m_mappedData;
		m_buffer = source.m_buffer;
		m_memory = source.m_memory;
		m_size = source.m_size;
		m_bufferInfo = source.m_bufferInfo;
		m_dynamicAlignment = source.m_dynamicAlignment;
        m_components.swap(source.m_components);
    
        source.ready = ready = false;

		source.m_mappedData = nullptr;
		source.m_buffer = nullptr;
		source.m_memory = nullptr;
		source.m_size = 0;
		source.m_bufferInfo = nullptr;
		source.m_dynamicAlignment = 0;

        puts("moved uniform");
    }

    ZUniform& ZUniform::operator=(ZUniform&& source) {
		m_mappedData = source.m_mappedData;
		m_buffer = source.m_buffer;
		m_memory = source.m_memory;
		m_size = source.m_size;
		m_bufferInfo = source.m_bufferInfo;
		m_dynamicAlignment = source.m_dynamicAlignment;
        m_components.swap(source.m_components);
    
        source.ready = ready = false;

		source.m_mappedData = nullptr;
		source.m_buffer = nullptr;
		source.m_memory = nullptr;
		source.m_size = 0;
		source.m_bufferInfo = nullptr;
		source.m_dynamicAlignment = 0;
        
        puts("moved uniform (=)");
        return *this;
    }

    void ZUniform::setDynamicAlignments()
	{
		VkPhysicalDeviceProperties devProps;
		vkGetPhysicalDeviceProperties(*ZDevice::getPhysicalDev(), &devProps);

		size_t minAligment = devProps.limits.minUniformBufferOffsetAlignment;

		m_dynamicAlignment = m_size;
		m_dynamicAlignment = (m_dynamicAlignment + minAligment - 1) & ~(minAligment - 1);

		printf("uniformDynamicAlignment: %ld\n", m_dynamicAlignment);
		printf("minUniformBufferOffsetAlignment: %zu\n", minAligment);
	}

	void ZUniform::create(uint32_t objectCount)
	{
		if(objectCount == 1)
		{
			createBuffer(
				ZDevice::getDevice(),
				m_size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				m_buffer,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_memory
			);

			m_bufferInfo = new VkDescriptorBufferInfo;
			m_bufferInfo->offset = 0;
			m_bufferInfo->range = m_size;
			m_bufferInfo->buffer = m_buffer;
		}
		else
		{
			setDynamicAlignments();

			m_size = m_dynamicAlignment * objectCount;

			// create dynamic uniformBuffer
			createBuffer(
				ZDevice::getDevice(),
				m_size,
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

		vkMapMemory(ZDevice::getDevice(), m_memory, 0, m_size, 0, &m_mappedData);

        ready = true;
	}

	ZUniform::~ZUniform()
	{
		vkDestroyBuffer(ZDevice::getDevice(), m_buffer, nullptr);
		vkFreeMemory(ZDevice::getDevice(), m_memory, nullptr);

        puts("Destroyed uniformBuffer");
	};
}
