#ifndef ZPRIMITIVE_H_
#define ZPRIMITIVE_H_

#include "types.h"

#include "Vulkan/Pipeline.h"
#include "Vulkan/Uniform.h"
#include "Vulkan/DescriptorPool.h"

namespace ZeroVulkan 
{
    class ZRect
    {
    public:
        ZRect(vec2 pos, float width, float height, vec4 color);
        ZRect(ZRect&& source);
        ZRect& operator=(ZRect&& source);
         ~ZRect();
         
        inline void setTopology(ZTopology topology) { pipeline.setTopolgy(topology); }
         
        void bind(VkCommandBuffer& cmdBuffer);
    private:
        vec4 m_color;
        vec2 m_pos;
        float m_width;
        float m_height;

        ZPipeline pipeline;

        ZUniform uniform;
        ZDescriptorPool descPool;

        VkShaderModule shaderModuleVert = nullptr;
        VkShaderModule shaderModuleFrag = nullptr;
    };  
}

#endif // ZPRIMITIVE_H_
