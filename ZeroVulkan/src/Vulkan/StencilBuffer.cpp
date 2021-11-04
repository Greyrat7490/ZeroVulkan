#include "StencilBuffer.h"
#include "ZShaderSet/Parser.h"
#include "ZMesh/ZMesh.h"
#include "ComputeShader.h"
#include "types.h"
#include <functional>
#include <vulkan/vulkan_core.h>

namespace ZeroVulkan
{
    void ZStencilBuffer::createModules() {
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

    ZStencilBuffer::ZStencilBuffer() {
        createModules();
        printf("created ZStencilBuffer\n");
    }

	ZStencilBuffer::~ZStencilBuffer()
	{
		vkDestroyShaderModule(ZDevice::getDevice(), m_vertShader, nullptr);
		vkDestroyShaderModule(ZDevice::getDevice(), m_fragShader, nullptr);

        m_uniforms.clear();

		printf("destroyed ZStencilBuffer\n");
	}

    void ZStencilBuffer::create(ZDescriptorPool& descPool) {
        descPool.addDescriptorLayout(m_pipeline.getDescLayout());

        m_pipeline.setShaders(m_vertShader, m_fragShader);
        m_pipeline.setVertexLayout(&m_vertexLayout);
        m_pipeline.setStencil(false);
        m_pipeline.create();
    }


    ZUniform& ZStencilBuffer::getUniform(size_t index) {
        ZASSERT_FUNC(index < m_uniforms.size(), "Stencil has not enough uniforms");

        return m_uniforms[index];
    }
}
