#include "ZPrimitvie.h"
#include "Vulkan/Shader.h"
#include "types.h"

namespace ZeroVulkan 
{
    ZRect::ZRect(vec2 pos, float width, float height, vec4 color) 
        : m_color(color), 
        m_pos(pos),
        m_width(width),
        m_height(height)
    {
        std::string absPath = pathToAbsolue("ZeroVulkan/shaders/rect.vert"); 
        createShaderModule(absPath, &shaderModuleVert);
        
        absPath = pathToAbsolue("ZeroVulkan/shaders/rect.frag"); 
        createShaderModule(absPath, &shaderModuleFrag);

        uniform.addComponent(ZType::VEC2);        
        uniform.addComponent(ZType::VEC2);        
        uniform.addComponent(ZType::VEC4);
        uniform.create();
        
        *uniform.getComponent<vec2>(0) = pos;
        *uniform.getComponent<vec2>(1) = vec2(width, height);
        *uniform.getComponent<vec4>(2) = color;

        descSetLayout.addBinding(0, uniform.getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        descSetLayout.create();

        descPool.addDescriptorLayout(&descSetLayout);
        descPool.create();

        descriptorSet.create(&descSetLayout, descPool.descriptorPool);
        
        pipeline.setShaders(shaderModuleVert, shaderModuleFrag);
        // TODO: move setLayout into create
        pipeline.setLayout(&descSetLayout.layout, 1);
        pipeline.create();
    }

    ZRect::ZRect(ZRect&& source) {
        m_color = source.m_color;
        m_pos = source.m_pos;
        m_width = source.m_width;
        m_height = source.m_height;

        pipeline = source.pipeline;

        uniform = std::move(source.uniform);
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;

        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        puts("moved ZRect");
    }

    ZRect& ZRect::operator=(ZRect&& source) {
        m_color = source.m_color;
        m_pos = source.m_pos;
        m_width = source.m_width;
        m_height = source.m_height;

        pipeline = source.pipeline;

        uniform = std::move(source.uniform);
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;

        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        puts("moved ZRect (=)");
        return *this;
    }
    
    ZRect::~ZRect() {
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleVert, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleFrag, nullptr);
    }
    
    void ZRect::bind(VkCommandBuffer& cmdBuffer) {
        pipeline.bind(cmdBuffer, &descriptorSet);
        vkCmdDraw(cmdBuffer, 6, 1, 0, 0);
    }
}
