#ifndef ZSHADERS_H_
#define ZSHADERS_H_

#include <string>
#include <vector>
#include "Vulkan/Uniform.h"
#include "Vulkan/Vertex.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/StencilBuffer.h"
#include "Vulkan/ComputeShader.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptorSetLayout.h"

namespace ZeroVulkan
{
    class ZShaderSet
    {
    public:
        ZShaderSet(const std::string& vertexShaderName, const std::string& fragmentShaderName);
        // ZShaderSet(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& texturePath);
        ~ZShaderSet();
        ZShaderSet(ZShaderSet&& source);

        ZShaderSet& operator=(ZShaderSet&& source);

        ZUniform& getUniform(size_t index = 0);       

        void create();

        // TODO: merge those two methods
        void setComputeShader(ZComputeShader* computeShader);
        void setShader(const std::string& path, ZShaderType type);
        inline void setTopology(ZTopology topology) { pipeline.setTopolgy(topology); }

        void bind(VkCommandBuffer& cmdBuffer);
        void buildComputeShader();
        void submitComputeShader();
    private:
        std::vector<ZUniform> uniforms;
        // ZStencilBuffer* stencilBuffer = nullptr;
        
        VkShaderModule m_vertShader = nullptr;
        VkShaderModule m_fragShader = nullptr;
        ZComputeShader* m_computeShader = nullptr;

        ZPipeline pipeline;

        ZDescriptorPool descPool;

        ZVertexLayout vertexLayout;

        bool ready = false;

        void parseVertShader(const std::string& path);
        void parseFragShader(const std::string& path);
        void parseCompShader(const std::string& path);
        
        void parseUniforms(const std::string& file);
        void parseVertexAttr(const std::string& file);  
    };
}

#endif // ZSHADERS_H_
