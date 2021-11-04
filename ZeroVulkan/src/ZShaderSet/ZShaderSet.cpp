#include <string>
#include "Vulkan/ComputeShader.h"
#include "Vulkan/Device.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Uniform.h"
#include "ZRenderer/ZRenderer.h"
#include "ZShaderSet.h"
#include "utils.h"
#include "Parser.h"

namespace ZeroVulkan
{   
    ZShaderSet::ZShaderSet(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string absPath = pathToAbsolue(vertexShaderPath); 
        createShaderModule(absPath, &m_vertShader);
        parseVertShader(absPath);

        absPath = pathToAbsolue(fragmentShaderPath); 
        createShaderModule(absPath, &m_fragShader);
        parseFragShader(absPath);
    }

    ZShaderSet::~ZShaderSet()
    {
        if (m_computeShader)
            delete m_computeShader;

        if (m_stencilBuffer)
            delete m_stencilBuffer;

        vkDestroyShaderModule(ZDevice::getDevice(), m_vertShader, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), m_fragShader, nullptr);

        m_uniforms.clear();

        printf("destroyed ZShaders\n");
    }


    void ZShaderSet::setComputeShader(ZComputeShader* computeShader) {
        m_computeShader = computeShader;
        ZRenderer::record();
    }

    ZShaderSet::ZShaderSet(ZShaderSet&& source) {
        m_uniforms.swap(source.m_uniforms);
        m_stencilBuffer = source.m_stencilBuffer;
        m_computeShader = source.m_computeShader;
        pipeline = source.pipeline;
        descPool = source.descPool;
        vertexLayout = source.vertexLayout;
        m_vertShader = source.m_vertShader;
        m_fragShader = source.m_fragShader;

        ready = false;

        source.m_stencilBuffer = nullptr;
        source.m_computeShader = nullptr;
        source.m_vertShader = nullptr;
        source.m_fragShader = nullptr;

        puts("moved shaders");
    }

    ZShaderSet& ZShaderSet::operator=(ZShaderSet&& source) {
        m_uniforms.swap(source.m_uniforms);
        m_stencilBuffer = source.m_stencilBuffer;
        m_computeShader = source.m_computeShader;
        pipeline = source.pipeline;
        descPool = source.descPool;
        vertexLayout = source.vertexLayout;
        m_vertShader = source.m_vertShader;
        m_fragShader = source.m_fragShader;

        ready = false;

        source.m_stencilBuffer = nullptr;
        source.m_computeShader = nullptr;
        source.m_vertShader = nullptr;
        source.m_fragShader = nullptr;

        puts("moved shaders (=)");
        return *this;
    }


    ZUniform& ZShaderSet::getUniform(size_t index) {
        ZASSERT_FUNC(index < m_uniforms.size(), "ShaderSet has not enough uniforms");

        return m_uniforms[index];
    }

    ZUniform& ZShaderSet::getStencilUniform(size_t index) {
        ZASSERT_FUNC(m_stencilBuffer, "this ShaderSet has no Stencil");

        return m_stencilBuffer->getUniform(index);
    }

    void ZShaderSet::create()
    {
        if (m_stencilBuffer) {
            pipeline.setStencil(true);

            m_stencilBuffer->create(descPool);
            descPool.maxSets++;
        }

        descPool.addDescriptorLayout(pipeline.getDescLayout());
        descPool.create();

        pipeline.setShaders(m_vertShader, m_fragShader);
        pipeline.setVertexLayout(&vertexLayout);
        pipeline.create();
        pipeline.createDescSet(descPool);

        if (m_stencilBuffer)
            m_stencilBuffer->createDescSet(descPool);

        ready = true;
        puts("created shaders");
    }

    void ZShaderSet::setShader(const std::string& path, ZShaderType type)
    {
        static_assert(SHADER_TYPE_COUNT == 3, "Exhaustive use of ZShaderType (add cases)");

        switch(type)
        {
        case ZShaderType::VERTEX: {
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &m_vertShader);
            parseVertShader(absPath);
        }
        break;
        case ZShaderType::FRAGMENT: {
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &m_fragShader);
            parseVertShader(absPath);
        }
        break;
        // TODO: Compute Shader support
        case ZShaderType::COMPUTE:
            ZASSERT_FUNC(false, "you have to use setComputeShader()");
            break;
        default:
            ZASSERT_FUNC(false, "unknown ShaderType");
            break;
        }
    }

    void ZShaderSet::parseVertShader(const std::string& path) {
        std::string file = readFile(path);
        trimFile(file);

        parseVertexAttr(&vertexLayout, file);
        parseBindings(m_uniforms, &pipeline, file);
    }

    void ZShaderSet::parseFragShader(const std::string& path) {
        std::string file = readFile(path);
        trimFile(file);

        parseBindings(m_uniforms, &pipeline, file);
    }

    void ZShaderSet::bind(VkCommandBuffer& cmdBuffer) {
        if (!ready)
            create();

        pipeline.bind(cmdBuffer);

        if (m_computeShader) {
            m_computeShader->bind(cmdBuffer);
            m_computeShader->drawCmd(cmdBuffer);
        }
    }

    bool ZShaderSet::bindStencil(VkCommandBuffer& cmdBuffer) {
        if (m_stencilBuffer) {
            m_stencilBuffer->bind(cmdBuffer);
            return true;
        }

        return false;
    }

    void ZShaderSet::buildComputeShader() {
        if (m_computeShader)
            m_computeShader->buildCommandBuffer();
    }

    void ZShaderSet::submitComputeShader() {
        if (m_computeShader)
            m_computeShader->submit(ZDevice::getQueue());
    }
}
