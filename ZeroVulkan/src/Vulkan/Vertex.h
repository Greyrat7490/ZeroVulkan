#ifndef H_VERTEX
#define H_VERTEX

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <algorithm>
#include "types.h"

namespace ZeroVulkan 
{
    class ZVertex
    {
    public:
        template<typename... Ts>
        inline ZVertex(Ts&& ... args)
        {
            addVertexData(args ...);
        }

        std::vector<float> vertexData;
    private:
        template<typename T, typename... Ts>
        void addVertexData(const T& first, Ts&& ... other);

        template<short size>
        void addVertexData(const vec<float, size>& v);

        void addVertexData(const float f);

        inline void addVertexData() {};
    };

    template<typename T, typename... Ts>
    inline void ZVertex::addVertexData(const T& first, Ts&& ... other)
    {
        addVertexData(first);

        addVertexData(other ...);
    }

    template<short size>
    inline void ZVertex::addVertexData(const vec<float, size>& v)
    {
        vertexData.reserve(size);

        for (uint32_t i = 0; i < size; i++)
           vertexData.push_back(v[i]);
    }

    inline void ZVertex::addVertexData(const float f)
    {
        vertexData.push_back(f);
    }


    class ZVertexLayout
    {
    public:
        ZVertexLayout() {};
        ~ZVertexLayout() {};

        inline void addLocation(uint32_t location, ZType type, uint32_t offset);
        inline void createBinding(uint32_t size);

        std::vector<VkVertexInputAttributeDescription> vertexAttributeDesc;
        VkVertexInputBindingDescription* bindingDescritption = nullptr;
    };

    inline void ZVertexLayout::createBinding(uint32_t size)
    {
        bindingDescritption = new VkVertexInputBindingDescription();
        bindingDescritption->binding = 0;
        bindingDescritption->stride = size;
        bindingDescritption->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    inline void ZVertexLayout::addLocation(uint32_t location, ZType type, uint32_t offset)
    {
        vertexAttributeDesc.resize(vertexAttributeDesc.size() + 1);

        vertexAttributeDesc.back().binding = 0;
        vertexAttributeDesc.back().location = location;
        vertexAttributeDesc.back().offset = offset;

        switch( type )
        {
        case ZType::VEC4:
            vertexAttributeDesc.back().format = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
        case ZType::VEC3:
            vertexAttributeDesc.back().format = VK_FORMAT_R32G32B32_SFLOAT;
            break;
        case ZType::VEC2:
            vertexAttributeDesc.back().format = VK_FORMAT_R32G32_SFLOAT;
            break;
        case ZType::FLOAT:
            vertexAttributeDesc.back().format = VK_FORMAT_R32_SFLOAT;
            break;
        default:
            printf("unknown vertexAttribute (ZType) %d\n", (int)type);
        }
    }
}

#endif // H_VERTEX
