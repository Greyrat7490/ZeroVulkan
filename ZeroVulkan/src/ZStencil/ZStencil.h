#ifndef ZSTENCILE_H_
#define ZSTENCILE_H_

#include <vector>

#include "types.h"

#include "Vulkan/Uniform.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Vertex.h"


namespace ZeroVulkan
{
    class ZMesh;
    class ZComputeShader;

    class ZStencil
    {
    public:
        ZStencil();
        ~ZStencil();

        ZUniform& getUniform(size_t index);

        inline void bind(VkCommandBuffer& cmdBuffer) { m_pipeline.bind(cmdBuffer); }

        void createDescSet(ZDescriptorPool& pool) { m_pipeline.createDescSet(pool); }
        void create(ZDescriptorPool& descPool);
    private:
        VkShaderModule m_vertShader;
        VkShaderModule m_fragShader;

        ZPipeline m_pipeline;
        std::vector<ZUniform> m_uniforms;
        ZVertexLayout m_vertexLayout;

        const ZMesh* m_mesh = nullptr;
        const ZComputeShader* m_compShader = nullptr;
        void createModules();
    };
}

#endif // ZSTENCILE_H_
