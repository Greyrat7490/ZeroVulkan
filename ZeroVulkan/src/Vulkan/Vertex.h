#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include <vulkan/vulkan_core.h>
#include "types.h"

namespace ZeroVulkan {
    class ZVertexLayout
    {
    public:
        inline void addLocation(uint32_t location, ZType type);
        inline void createBinding();

        std::vector<VkVertexInputAttributeDescription> vertexAttributeDesc;
        VkVertexInputBindingDescription* bindingDescritption = nullptr;
    private:
        uint32_t m_offset = 0;
    };


    void ZVertexLayout::createBinding()
    {
        bindingDescritption = new VkVertexInputBindingDescription();
        bindingDescritption->binding = 0;
        bindingDescritption->stride = m_offset; // m_offset = size
        bindingDescritption->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    void ZVertexLayout::addLocation(uint32_t location, ZType type)
    {
        vertexAttributeDesc.resize(vertexAttributeDesc.size() + 1);
        VkVertexInputAttributeDescription& desc = vertexAttributeDesc.back();
        
        desc.binding = 0;
        desc.location = location;
        desc.offset = m_offset;

        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new cases)");

        switch (type)
        {
        case ZType::MAT4:
            desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            m_offset += sizeof(mat4);
            break;
        case ZType::VEC4:
            desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            m_offset += sizeof(vec4);
            break;
        case ZType::VEC3:
            desc.format = VK_FORMAT_R32G32B32_SFLOAT;
            m_offset += sizeof(vec3);
            break;
        case ZType::VEC2:
            desc.format = VK_FORMAT_R32G32_SFLOAT;
            m_offset += sizeof(vec2);
            break;
        case ZType::FLOAT:
            desc.format = VK_FORMAT_R32_SFLOAT;
            m_offset += sizeof(float);
            break;
        default:
            printf("unknown vertexAttribute %s\n", zTypeToStr(type));
        }
    }
}

#endif // VERTEX_H_
