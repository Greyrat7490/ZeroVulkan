#include <assert.h>
#include "Vulkan/ComputeShader.h"
#include "Vulkan/Shader.h"
#include "ZShaderSet.h"
#include "utils.h"

namespace ZeroVulkan
{
    ZShaderSet::ZShaderSet(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        // TODO own assert
        printf("vertexShader: %s\n", vertexShaderPath.c_str());
        printf("fragmentShader: %s\n", fragmentShaderPath.c_str());

        createShaderModule(pathToAbsolue(vertexShaderPath), &shaderModuleVert);
        createShaderModule(pathToAbsolue(fragmentShaderPath), &shaderModuleFrag);
    }

    ZShaderSet::~ZShaderSet()
    {
        if (stencilBuffer)
            delete stencilBuffer;

        if (ready) {
            vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
            vkDestroyPipelineLayout(ZDevice::getDevice(), pipelineLayout, nullptr);
        }

        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleVert, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleFrag, nullptr);

        printf("destroyed ZShaders\n");
    }

    ZShaderSet::ZShaderSet(ZShaderSet&& source) {
        uniform = std::move(source.uniform);
        uniformLayout = std::move(source.uniformLayout);
        stencilBuffer = source.stencilBuffer;
        pipeline = source.pipeline;
        pipelineLayout = source.pipelineLayout;
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;
        vertexLayout = source.vertexLayout;
        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        ready = false;

        source.stencilBuffer = nullptr;
        source.pipeline = nullptr;
        source.pipelineLayout = nullptr;
        source.shaderModuleVert = nullptr;
        source.shaderModuleFrag = nullptr;

        puts("moved shaders");
    }

    ZShaderSet& ZShaderSet::operator=(ZShaderSet&& source) {
        uniform = std::move(source.uniform);
        uniformLayout = std::move(source.uniformLayout);
        stencilBuffer = source.stencilBuffer;
        pipeline = source.pipeline;
        pipelineLayout = source.pipelineLayout;
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;
        vertexLayout = source.vertexLayout;
        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        ready = false;

        source.stencilBuffer = nullptr;
        source.pipeline = nullptr;
        source.pipelineLayout = nullptr;
        source.shaderModuleVert = nullptr;
        source.shaderModuleFrag = nullptr;

        puts("moved shaders (=)");
        return *this;
    }


    void ZShaderSet::update() {
        uniform.update();
    }

    void ZShaderSet::create(bool debug, bool triangleTopology)
    {
        descSetLayout.createLayout();
        descPool.addDescriptorLayout(&descSetLayout);

        if (debug)
        {
            stencilBuffer = new ZStencilBuffer();
            descPool.addDescriptorLayout(stencilBuffer->getOutlineDescSetLayout());
            descPool.create(2);
        }
        else
            descPool.create();

        descriptorSet.setLayout(&descSetLayout);

        for (uint32_t i = 0; i < descSetLayout.getBindings().size(); i++)
        {
            switch(descSetLayout.getBindings()[i].descriptorType)
            {
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                descriptorSet.addDescriptorInfo(i, uniform.getBufferInfo());
                break;
            default:
                printf("DescriptorTyp is not yet supported\n");
                break;
            }
        }

        descriptorSet.create(descPool.descriptorPool);

        if(debug)
        {
            stencilBuffer->createDescSet(descPool.descriptorPool);
            stencilBuffer->createPipelines(&vertexLayout, shaderModuleVert, shaderModuleFrag, &descSetLayout.layout, 1);
        }
        else
        {
            createPipelineLayout(pipelineLayout, &descSetLayout.layout, 1);

            if(triangleTopology)
            {
                createGraphicsPipeline(
                    pipeline,
                    pipelineLayout,
                    shaderModuleVert,
                    shaderModuleFrag,
                    &vertexLayout
                );
            }
            else
            {
                createParticleGraphicsPipeline(
                    pipeline,
                    pipelineLayout,
                    shaderModuleVert,
                    shaderModuleFrag,
                    &vertexLayout
                );
            }
        }
        
        puts("created shaders");
    }

    void ZShaderSet::setShader(const std::string& shaderName, ZShaderType type)
    {
        printf("shader: %s\n", ("Test/shader/compiled/" + shaderName).c_str());

        static_assert(SHADER_TYPE_COUNT == 3, "Exhaustive use of ZShaderType (add cases)");
        
        switch(type)
        {
        case ZShaderType::VERTEX:
            createShaderModule("Test/shader/compiled/" + shaderName, &shaderModuleVert);
            break;
        case ZShaderType::FRAGMENT:
            createShaderModule("Test/shader/compiled/" + shaderName, &shaderModuleFrag);
            break;
        // TODO: Compute Shader support
        case ZShaderType::COMPUTE:
            puts("Computer Shader are not yet supported");
            assert(false && "Computer Shader are not yet supported");
            break;
        default:
            puts("unknown ShaderType");
            break;
        }
    }
    
    void ZShaderSet::bind(VkCommandBuffer& cmdBuffer) {
        prepair();
            
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet.descSet, 0, 0);
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }
    
    void ZShaderSet::prepair() {
        if (!ready) {
            uniform.create(&uniformLayout);
            create(false);
            ready = true;
        }
    }
}
