#ifndef ZPIPELINE_H_
#define ZPIPELINE_H_

#include "Device.h"
#include "Shader.h"
#include "Vertex.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
#include "Vulkan/DescriptorSet.h"

namespace ZeroVulkan
{
    class ZPipeline {
    public:
        ZPipeline();
        ~ZPipeline();
        
        void setLayout(const VkDescriptorSetLayout* descriptorLayouts, uint32_t descriptorLayoutCount);
        void setShaders(VkShaderModule shaderModuleVert, VkShaderModule shaderModuleFrag);
        void setVertexLayout(ZVertexLayout* vertexLayout);
        void setTopolgy(ZTopology topology);
        
        void create();
        void recreate();

        // TODO: move descSet into pipeline
        void bind(VkCommandBuffer& cmdBuffer, ZDescriptorSet* descSet);
        
    private:
        bool ready = false;
        
        VkPipeline pipeline = nullptr;
        VkPipelineLayout layout = nullptr;
        
        uint32_t shaderStagesCount = 0;
        VkPipelineShaderStageCreateInfo* shaderStages = nullptr;
        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    };
}
#endif // PIPELINE_H_
