#ifndef ZSHADERS_H_
#define ZSHADERS_H_

#include <string>
#include <vulkan/vulkan_core.h>
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/Vertex.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/StencilBuffer.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptorSetLayout.h"

namespace ZeroVulkan
{
    class ZShaders
    {
    public:
        ZShaders(const std::string& vertexShaderName, const std::string& fragmentShaderName);
        ZShaders(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& texturePath);
        ~ZShaders();
        ZShaders(ZShaders&& source);

        ZShaders& operator=(ZShaders&& source);

        void update(float deltaTime);

        void create(bool debug, bool triangleTopology = true);

        void setShader(const std::string& shaderName, shaderType type);

        void prepair();
        void bind(VkCommandBuffer& cmdBuffer);

        ZUniform uniform;
        ZStencilBuffer* stencilBuffer = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        ZDescriptorPool descPool;
        ZDescriptorSetLayout descSetLayout;
        ZDescriptorSet descriptorSet;

        ZVertexLayout vertexLayout;

        float counter = 0; // for shader animations
    private:
        VkShaderModule shaderModuleVert = nullptr;
        VkShaderModule shaderModuleFrag = nullptr;
        bool ready = false;
    };
}

#endif // ZSHADERS_H_
