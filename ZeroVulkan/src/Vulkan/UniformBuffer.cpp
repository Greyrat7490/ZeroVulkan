#include "UniformBuffer.h"
#include <assert.h>

namespace ZeroVulkan {
    std::any* ZUniformLayout::addComponent(ZType type) {
        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new cases)");
        
        switch (type)
        {
        case ZType::MAT4:
            m_components.push_back(std::make_pair(ZType::MAT4, mat4(1)));
            m_uniformSize += sizeof(mat4);
            break;
        case ZType::VEC4:
            m_components.push_back(std::make_pair(ZType::VEC4, vec4()));
            m_uniformSize += sizeof(vec4);
            break;
        case ZType::VEC3:
            m_components.push_back(std::make_pair(ZType::VEC3, vec3(1.f, 1.f, 3.f)));
            m_uniformSize += sizeof(vec3);
            break;
        case ZType::VEC2:
            m_components.push_back(std::make_pair(ZType::VEC2, vec2()));
            m_uniformSize += sizeof(vec2);
            break;
        case ZType::FLOAT:
            m_components.push_back(std::make_pair(ZType::FLOAT, 0.f));
            m_uniformSize += sizeof(float);
            break;
        default:
            printf("unknown ZType %d\n", (int)type);
        }

        return &m_components.back().second;
    }
    
    ZUniformLayout::ZUniformLayout(ZUniformLayout&& source) {
        m_uniformSize = source.m_uniformSize;
        m_bufferInfo = source.m_bufferInfo;
        m_components.swap(source.m_components);
        
        source.m_uniformSize = 0;
        source.m_bufferInfo = nullptr;
        
        puts("moved uniform layout");
    }

    ZUniformLayout& ZUniformLayout::operator=(ZUniformLayout&& source) {    
        m_uniformSize = source.m_uniformSize;
        m_bufferInfo = source.m_bufferInfo;
        m_components.swap(source.m_components);
        
        source.m_uniformSize = 0;
        source.m_bufferInfo = nullptr;

        puts("moved uniform layout (=)");
        return *this;
    }

    ZUniform::ZUniform(ZUniform&& source) {
		mappedData = source.mappedData;
		m_layout = source.m_layout;
		m_buffer = source.m_buffer;
		m_memory = source.m_memory;
		m_uniformSize = source.m_uniformSize;
		m_bufferInfo = source.m_bufferInfo;
		m_dynamicAlignment = source.m_dynamicAlignment;
    
        source.ready = ready = false;

		source.mappedData = nullptr;
		source.m_buffer = nullptr;
		source.m_memory = nullptr;
		source.m_uniformSize = 0;
		source.m_bufferInfo = nullptr;
		source.m_dynamicAlignment = 0;

        puts("moved uniform");
    }

    ZUniform& ZUniform::operator=(ZUniform&& source) {
		mappedData = source.mappedData;
		m_layout = source.m_layout;
		m_buffer = source.m_buffer;
		m_memory = source.m_memory;
		m_uniformSize = source.m_uniformSize;
		m_bufferInfo = source.m_bufferInfo;
		m_dynamicAlignment = source.m_dynamicAlignment;
    
        source.ready = ready = false;

		source.mappedData = nullptr;
		source.m_layout = nullptr;
		source.m_buffer = nullptr;
		source.m_memory = nullptr;
		source.m_uniformSize = 0;
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

		m_dynamicAlignment = m_layout->getSize();
		m_dynamicAlignment = (m_dynamicAlignment + minAligment - 1) & ~(minAligment - 1);

		printf("uniformDynamicAlignment: %ld\n", m_dynamicAlignment);
		printf("minUniformBufferOffsetAlignment: %zu\n", minAligment);
	}

	void ZUniform::create(ZUniformLayout* layout, uint32_t objectCount)
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

        ready = true;
	}

	ZUniform::~ZUniform()
	{
		vkDestroyBuffer(ZDevice::getDevice(), m_buffer, nullptr);
		vkFreeMemory(ZDevice::getDevice(), m_memory, nullptr);

        puts("Destroyed uniformBuffer");
	};

	void ZUniform::update()
	{
        assert(ready);
        
        // TODO: maybe just one memcpy
        char* ptr = (char*)mappedData;
        for (std::pair<ZType, std::any>& c : m_layout->m_components) {
            static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new cases)");
            
            switch (c.first)
            {
            case ZType::MAT4:
                memcpy(ptr, std::any_cast<mat4>(&c.second), sizeof(mat4));
                ptr += sizeof(mat4);
                break;
            case ZType::VEC4:
                memcpy(ptr, std::any_cast<vec4>(&c.second), sizeof(vec4));
                ptr += sizeof(vec4);
                break;
            case ZType::VEC3:
                memcpy(ptr, std::any_cast<vec3>(&c.second), sizeof(vec3));
                ptr += sizeof(vec3);
                break;
            case ZType::VEC2:
                memcpy(ptr, std::any_cast<vec2>(&c.second), sizeof(vec2));
                ptr += sizeof(vec2);
                break;
            case ZType::FLOAT:
                memcpy(ptr, std::any_cast<float>(&c.second), sizeof(float));
                ptr += sizeof(float);
                break;
            default:
                printf("unknown ZType %d\n", (int)c.first);
            }
        }
	}
}
