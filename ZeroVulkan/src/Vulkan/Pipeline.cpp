#include "Pipeline.h"
#include "Window/window.h"

namespace ZeroVulkan {
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
    
    void ZPipeline::setLayout()
    {
        if (!descSetLayout.getBindings().empty()) {
            descSetLayout.create();
            
            VkPipelineLayoutCreateInfo layoutCreateInfo = {};
            layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            layoutCreateInfo.pSetLayouts = &descSetLayout.layout;
            layoutCreateInfo.setLayoutCount = 1;
            layoutCreateInfo.pushConstantRangeCount = 0;
            layoutCreateInfo.pPushConstantRanges = nullptr;

            VkResult res = vkCreatePipelineLayout(ZDevice::getDevice(), &layoutCreateInfo, nullptr, &layout);
            if (res != VK_SUCCESS) 
                printf("create pipelineLayout ERROR: %d\n", res);
        }
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

    void ZPipeline::setTopolgy(ZTopology topology) {
        static_assert(ZTOPOLOGY_COUNT == 3, "exhaustive use of ZTopology (added cases to switch)");
        
        switch (topology) {
            case ZTopology::TRIANGLE:
                inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
                break;
            case ZTopology::LINE:
                inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                rasterizationState.cullMode = VK_CULL_MODE_NONE;
                break;
            case ZTopology::POINT:
                inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                rasterizationState.cullMode = VK_CULL_MODE_NONE;
                break;
            default:
                puts("unreachable");
                exit(1);
        }

        if (ready)
            recreate();
    }
    
    void ZPipeline::bind(VkCommandBuffer& cmdBuffer) {
        if (!ready)
            create();
            
        if (layout && descriptorSet.descSet)
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet.descSet, 0, 0);

        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void ZPipeline::addBinding(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags) {
        descSetLayout.addBinding(binding, bufferInfo, descriptorType, stageFlags);
    }

    void ZPipeline::addBinding(uint32_t binding, const VkDescriptorImageInfo* imageInfo, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags) {
        descSetLayout.addBinding(binding, imageInfo, descriptorType, stageFlags);
    }
    
    
    void ZPipeline::recreate() {
        vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
        create();
    }

    void ZPipeline::create()
    {
        setLayout();
        
        vec2 winSize = ZWindow::getSize();
        
        VkViewport viewport;
        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.width = winSize[0];
        viewport.height = winSize[1];
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        VkRect2D scissor;
        scissor.offset = { 0, 0 };
        scissor.extent = { (uint32_t) winSize[0], (uint32_t) winSize[1] };

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

        ready = true;
    }

    void ZPipeline::createDescSet(ZDescriptorPool& pool) {
        descriptorSet.create(&descSetLayout, pool.descriptorPool);
    }
}
