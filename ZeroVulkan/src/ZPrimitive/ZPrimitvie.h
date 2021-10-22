#ifndef ZPRIMITIVE_H_
#define ZPRIMITIVE_H_

#include "types.h"
#include "Vulkan/Uniform.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptorSetLayout.h"

namespace ZeroVulkan 
{
    class ZRect
    {
    public:
        ZRect(vec2 pos, float width, float height, vec4 color);
        ZRect(ZRect&& source);
        ZRect& operator=(ZRect&& source);
         ~ZRect();
         
         
        void bind(VkCommandBuffer& cmdBuffer);
    private:
        vec4 m_color;
        vec2 m_pos;
        float m_width;
        float m_height;

        ZPipeline pipeline;

        ZUniform uniform;
        ZDescriptorPool descPool;
        ZDescriptorSetLayout descSetLayout;
        ZDescriptorSet descriptorSet;

        VkShaderModule shaderModuleVert = nullptr;
        VkShaderModule shaderModuleFrag = nullptr;
    };  
}

#endif // ZPRIMITIVE_H_
