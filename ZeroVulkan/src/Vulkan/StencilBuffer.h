#ifndef H_STENCILE_BUFFER
#define H_STENCILE_BUFFER

#include <functional>
#include <vector>
#include "Uniform.h"
#include "Pipeline.h"
#include "Vulkan/ComputeShader.h"
#include "Vulkan/Vertex.h"
#include "types.h"

namespace ZeroVulkan
{
    class ZMesh;
    class ZComputeShader;

    class ZStencilBuffer
    {
    public:
        ZStencilBuffer();
        ~ZStencilBuffer();

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

#endif // H_STENCILE_BUFFER
