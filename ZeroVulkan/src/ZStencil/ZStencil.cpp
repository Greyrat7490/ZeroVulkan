#include "ZStencil.h"

#include "types.h"
#include "ZShaderSet/Parser.h"

#include "Vulkan/Shader.h"
#include "Vulkan/DescriptorPool.h"


namespace ZeroVulkan
{
    void ZStencil::createModules() {
        std::string absPath = pathToAbsolue("ZeroVulkan/shaders/outline.vert"); 
        createShaderModule(absPath, &m_vertShader);
        std::string file = readFile(absPath);
        trimFile(file);
        parseVertexAttr(&m_vertexLayout, file);
        parseBindings(m_uniforms, &m_pipeline, file);


        absPath = pathToAbsolue("ZeroVulkan/shaders/outline.frag"); 
        file = readFile(absPath);
        trimFile(file);
        createShaderModule(absPath, &m_fragShader);
        parseBindings(m_uniforms, &m_pipeline, file);
    }

    ZStencil::ZStencil() {
        createModules();
        printf("created ZStencilBuffer\n");
    }

    ZStencil::~ZStencil()
    {
        vkDestroyShaderModule(ZDevice::getDevice(), m_vertShader, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), m_fragShader, nullptr);

        m_uniforms.clear();

        printf("destroyed ZStencilBuffer\n");
    }

    void ZStencil::create(ZDescriptorPool& descPool) {
        descPool.addDescriptorLayout(m_pipeline.getDescLayout());

        m_pipeline.setShaders(m_vertShader, m_fragShader);
        m_pipeline.setVertexLayout(&m_vertexLayout);
        m_pipeline.setStencil(false);
        m_pipeline.create();
    }


    ZUniform& ZStencil::getUniform(size_t index) {
        ZASSERT_FUNC(index < m_uniforms.size(), "Stencil has not enough uniforms");

        return m_uniforms[index];
    }
}
