#include <assert.h>
#include <vulkan/vulkan_core.h>
#include "Vulkan/ComputeShader.h"
#include "ZShaders.h"
#include "utils.h"

namespace ZeroVulkan
{
    // TODO handle relative and absolute path
    ZShaders::ZShaders(const std::string& vertexShaderRelPath, const std::string& fragmentShaderRelPath)
    {
        vertexLayout = new ZVertexLayout();
        descSetLayout = new ZDescriptorSetLayout();
        descriptorSet = new ZDescriptorSet();
        descPool = new ZDescriptorPool();

        // TODO own assert
        assert(vertexShaderRelPath[0] != '/');
        assert(fragmentShaderRelPath[0] != '/');
        
        printf("vertexShader: %s\n", vertexShaderRelPath.c_str());
        printf("fragmentShader: %s\n", fragmentShaderRelPath.c_str());

        createShaderModule( getRootDir() + "/" + vertexShaderRelPath, &shaderModuleVert);
        createShaderModule( getRootDir() + "/" + fragmentShaderRelPath, &shaderModuleFrag);
        
        printf("component: ZShaders\n");
    }

    ZShaders::~ZShaders()
    {
        delete vertexLayout;
        delete descSetLayout;
        delete descPool;

        delete stencilBuffer;

        vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
        vkDestroyPipelineLayout(ZDevice::getDevice(), pipelineLayout, nullptr);


        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleVert, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleFrag, nullptr);

        printf("destroyed ZShaders\n");
    }

    void ZShaders::update(float deltaTime)
    {
        counter += deltaTime;
    }

    void ZShaders::create(bool debug, bool triangleTopology)
    {
        descSetLayout->createLayout();
        descPool->addDescriptorLayout(descSetLayout);

        if (debug)
        {
            stencilBuffer = new ZStencilBuffer();
            descPool->addDescriptorLayout(stencilBuffer->getOutlineDescSetLayout());
            descPool->create(2);
        }
        else
            descPool->create();

        descriptorSet->setLayout(descSetLayout);

        for (uint32_t i = 0; i < descSetLayout->getBindings().size(); i++)
        {
            switch(descSetLayout->getBindings()[i].descriptorType)
            {
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                descriptorSet->addDescriptorInfo(i, uniform.getBufferInfo());
                break;
            default:
                printf("DescriptorTyp is not yet supported\n");
                break;
            }
        }

        descriptorSet->create(descPool->descriptorPool);

        if(debug)
        {
            stencilBuffer->createDescSet(descPool->descriptorPool);
            stencilBuffer->createPipelines(vertexLayout, shaderModuleVert, shaderModuleFrag, &descSetLayout->layout, 1);
        }
        else
        {
            createPipelineLayout(pipelineLayout, &descSetLayout->layout, 1);

            if(triangleTopology)
            {
                createGraphicsPipeline(
                    pipeline,
                    pipelineLayout,
                    shaderModuleVert,
                    shaderModuleFrag,
                    vertexLayout
                );
            }
            else
            {
                createParticleGraphicsPipeline(
                    pipeline,
                    pipelineLayout,
                    shaderModuleVert,
                    shaderModuleFrag,
                    vertexLayout
                );
            }
        }
    }

    void ZShaders::setShader(const std::string& shaderName, shaderType type)
    {
        printf("shader: %s\n", ("Test/shader/compiled/" + shaderName).c_str());

        switch(type)
        {
        case shaderType::VERTEX:
            createShaderModule("Test/shader/compiled/" + shaderName, &shaderModuleVert);
            break;
        case shaderType::FRAGMENT:
            createShaderModule("Test/shader/compiled/" + shaderName, &shaderModuleFrag);
            break;
        case shaderType::COMPUTE:
            printf("Computer Shader are not yet supported\n");
            break;
        default:
            printf("unknown ShaderType\n");
            break;
        }
    }
    
    void ZShaders::bind(VkCommandBuffer& cmdBuffer) {
        prepair();
            
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet->descSet, 0, 0);
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }
    
    void ZShaders::prepair() {
        if (!ready){
            vertexLayout->addLocation(0, ZType::VEC3, 0);
            vertexLayout->addLocation(1, ZType::VEC3, sizeof(vec3));
            vertexLayout->createBinding(2 * sizeof(vec3));

            descSetLayout->addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);

            ZUniformLayout uniformLayout = ZUniformLayout( {
                sizeof(mat4),
                sizeof(mat4),
                sizeof(mat4),
                sizeof(vec3),
            } );
            
            uniform.create(&uniformLayout);
            create(false);
            
            ready = true;
        }
    }
}
