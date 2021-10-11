#ifndef ZSHADERS_H_
#define ZSHADERS_H_

#include <string>
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/Vertex.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/StencilBuffer.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptorSetLayout.h"

namespace ZeroVulkan
{
    class ZShaderSet
    {
    public:
        ZShaderSet(const std::string& vertexShaderName, const std::string& fragmentShaderName);
        ZShaderSet(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& texturePath);
        ~ZShaderSet();
        ZShaderSet(ZShaderSet&& source);

        ZShaderSet& operator=(ZShaderSet&& source);

        void update();

        void create(bool debug, bool triangleTopology = true);

        void setShader(const std::string& shaderName, ZShaderType type);

        void prepair();
        void bind(VkCommandBuffer& cmdBuffer);

        ZUniformLayout uniformLayout;
    private:
        ZUniform uniform;
        ZStencilBuffer* stencilBuffer = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = nullptr;

        ZDescriptorPool descPool;
        ZDescriptorSetLayout descSetLayout;
        ZDescriptorSet descriptorSet;

        ZVertexLayout vertexLayout;

        VkShaderModule shaderModuleVert = nullptr;
        VkShaderModule shaderModuleFrag = nullptr;
        bool ready = false;

        void parseVertShader(const std::string& path);
        void parseFragShader(const std::string& path);
    };
}

#endif // ZSHADERS_H_
