#ifndef ZSHADERS_H_
#define ZSHADERS_H_

#include <string>
#include <vector>
#include "Vulkan/Uniform.h"
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

        ZUniform& getUniform(size_t index = 0);       

        void create(bool triangleTopology = true);

        void setShader(const std::string& path, ZShaderType type);

        void bind(VkCommandBuffer& cmdBuffer);
    private:
        std::vector<ZUniform> uniforms;
        // ZStencilBuffer* stencilBuffer = nullptr;

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
        void parseUniforms(const std::string& file);
        void parseVertexAttr(const std::string& file);
        bool addComponentByStr(ZUniform& uniform, const std::string& str);
    };
}

#endif // ZSHADERS_H_
