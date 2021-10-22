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
        inline ZPipeline();
        inline ~ZPipeline();
        
        inline void setLayout(const VkDescriptorSetLayout* descriptorLayouts, uint32_t descriptorLayoutCount);
        inline void setShaders(VkShaderModule shaderModuleVert, VkShaderModule shaderModuleFrag);
        inline void setVertexLayout(ZVertexLayout* vertexLayout);
        inline void setTopolgy(bool triangle);
        
        inline void create();

        // TODO: move descSet into pipeline
        inline void bind(VkCommandBuffer& cmdBuffer, ZDescriptorSet* descSet);
        
    private:
        VkPipeline pipeline = nullptr;
        VkPipelineLayout layout = nullptr;
        
        uint32_t shaderStagesCount = 0;
        VkPipelineShaderStageCreateInfo* shaderStages = nullptr;
        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    };

    ZPipeline::ZPipeline() {
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.primitiveRestartEnable = false;
        inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.depthClampEnable = false;
        rasterizationState.rasterizerDiscardEnable = false;
        rasterizationState.depthBiasEnable = false;
        rasterizationState.depthBiasConstantFactor = 0.f;
        rasterizationState.depthBiasClamp = 0.f;
        rasterizationState.depthBiasSlopeFactor = 0.f;
        rasterizationState.lineWidth = 1.f;
    }
    
    ZPipeline::~ZPipeline() {
        vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
        vkDestroyPipelineLayout(ZDevice::getDevice(), layout, nullptr);
    }
    
    void ZPipeline::setLayout(const VkDescriptorSetLayout* descriptorLayouts, uint32_t descriptorLayoutCount)
    {
        VkPipelineLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pSetLayouts = descriptorLayouts;
        layoutCreateInfo.setLayoutCount = descriptorLayoutCount;
        layoutCreateInfo.pushConstantRangeCount = 0;
        layoutCreateInfo.pPushConstantRanges = nullptr;

        VkResult res = vkCreatePipelineLayout(ZDevice::getDevice(), &layoutCreateInfo, nullptr, &layout);
        if (res != VK_SUCCESS) 
            printf("create pipelineLayout ERROR: %d\n", res);
    }

    void ZPipeline::setShaders(VkShaderModule shaderModuleVert, VkShaderModule shaderModuleFrag) {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert = {};
        shaderStageCreateInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageCreateInfoVert.module = shaderModuleVert;
        shaderStageCreateInfoVert.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag = {};
        shaderStageCreateInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageCreateInfoFrag.module = shaderModuleFrag;
        shaderStageCreateInfoFrag.pName = "main";

        shaderStages = new VkPipelineShaderStageCreateInfo[] {
            shaderStageCreateInfoVert, shaderStageCreateInfoFrag
        };

        shaderStagesCount = 2;
    }

    void ZPipeline::setVertexLayout(ZVertexLayout* vertexLayout) {
        if (vertexLayout && !vertexLayout->vertexAttributeDesc.empty()) {
            vertexInputState.vertexBindingDescriptionCount = 1;
            vertexInputState.pVertexBindingDescriptions = vertexLayout->bindingDescritption;
            vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexLayout->vertexAttributeDesc.size());
            vertexInputState.pVertexAttributeDescriptions = vertexLayout->vertexAttributeDesc.data();
        }
    }

    // TODO: topology type
    void ZPipeline::setTopolgy(bool triangle) {
        if (triangle)
            inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        else {
            inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            rasterizationState.cullMode = VK_CULL_MODE_NONE;
        }
        
        // points, line
        // rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
        
        // triangle, outline 
        // rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    }
    
    void ZPipeline::bind(VkCommandBuffer& cmdBuffer, ZDescriptorSet* descSet) {
        if (layout && descSet)
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descSet->descSet, 0, 0);

        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void ZPipeline::create()
    {
        VkViewport viewport;
        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.width = (float) 800;         // TODO parameterize
        viewport.height = (float) 600;        // TODO parameterize
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        VkRect2D scissor;
        scissor.offset = { 0, 0 };
        scissor.extent = { 800, 600 };          // TODO parameterize

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.pNext = nullptr;
        viewportStateCreateInfo.flags = 0;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.pViewports = &viewport;
        viewportStateCreateInfo.scissorCount = 1;
        viewportStateCreateInfo.pScissors = &scissor;
       
        //Antianalysing
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.sampleShadingEnable = false;
        multisampleStateCreateInfo.minSampleShading = 1.f;
        multisampleStateCreateInfo.pSampleMask = nullptr;
        multisampleStateCreateInfo.alphaToCoverageEnable = false;
        multisampleStateCreateInfo.alphaToOneEnable = false;

        VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
        colorBlendAttachmentState.blendEnable = true;
        colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState.colorWriteMask = 0xf;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
        colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCreateInfo.logicOpEnable = false;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
        colorBlendStateCreateInfo.blendConstants[0] = 0.f;
        colorBlendStateCreateInfo.blendConstants[1] = 0.f;
        colorBlendStateCreateInfo.blendConstants[2] = 0.f;
        colorBlendStateCreateInfo.blendConstants[3] = 0.f;

        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
        pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
        pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates;

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = true;
        depthStencil.depthWriteEnable = true;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = false;
        depthStencil.stencilTestEnable = false;
        depthStencil.minDepthBounds = 0.f;
        depthStencil.maxDepthBounds = 1.f;


        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = shaderStagesCount;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputState;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineCreateInfo.pTessellationState = nullptr;
        pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizationState;
        pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        pipelineCreateInfo.pDepthStencilState = &depthStencil;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
        pipelineCreateInfo.layout = layout;
        pipelineCreateInfo.renderPass = ZDevice::getRenderPass();
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;

        VkResult res = vkCreateGraphicsPipelines(ZDevice::getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);
        if (res != VK_SUCCESS)
            printf("create graphicsPiplines ERROR: %d\n", res);
    }

    inline void createComputePipeline(VkPipeline& pipeline, VkPipelineLayout pipelineLayout, VkShaderModule shaderModule)
    {
        VkPipelineShaderStageCreateInfo shaderStage = {};
        shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        shaderStage.module = shaderModule;
        shaderStage.pName = "main";

        VkComputePipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stage = shaderStage;
        pipelineCreateInfo.layout = pipelineLayout;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;

        VkResult res = vkCreateComputePipelines(ZDevice::getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);
        if (res != VK_SUCCESS)
            printf("create computer pipline ERROR: %d\n", res); 
    }
}

#endif // PIPELINE_H_
