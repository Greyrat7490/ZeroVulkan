#ifndef ZPIPELINE_H_
#define ZPIPELINE_H_

#include "types.h"

#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"


namespace ZeroVulkan
{
    class ZDescriptorPool;
    class ZVertexLayout;
    
    class ZPipeline {
    public:
        ZPipeline();
        ~ZPipeline();

        inline const ZDescriptorSetLayout* getDescLayout() const { return &descSetLayout; }

        void setShaders(VkShaderModule shaderModuleVert, VkShaderModule shaderModuleFrag);
        void setVertexLayout(ZVertexLayout* vertexLayout);
        void setTopolgy(ZTopology topology);
        // TODO: void setPolygoneMode(ZPolygonMode mode);
        void setStencil(bool fill);
 
        void addBinding(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
        void addBinding(uint32_t binding, const VkDescriptorImageInfo* imageInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
        
        void create();
        void recreate();
        void createDescSet(ZDescriptorPool& pool);
        
        void bind(VkCommandBuffer& cmdBuffer);
        
    private:
        void setLayout();

        bool ready = false;
        
        VkPipeline pipeline = nullptr;
        VkPipelineLayout layout = nullptr;
        
        ZDescriptorSetLayout descSetLayout;
        ZDescriptorSet descriptorSet;
        
        uint32_t shaderStagesCount = 0;
        VkPipelineShaderStageCreateInfo* shaderStages = nullptr;
        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    };
}
#endif // PIPELINE_H_
