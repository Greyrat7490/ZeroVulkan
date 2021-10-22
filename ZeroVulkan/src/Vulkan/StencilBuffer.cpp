#include "StencilBuffer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "DescriptorSetLayout.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace ZeroVulkan
{
	ZStencilBuffer::ZStencilBuffer()
	{
		createUniform();

        m_outlineDescLayout = new ZDescriptorSetLayout();

		m_outlineDescLayout->addBinding(0, m_uniform->getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
		m_outlineDescLayout->create();

		createShaderModule(m_outlineVertPath, &m_outlineVertModule);
		createShaderModule(m_outlineFragPath, &m_outlineFragModule);

        printf("created ZStencilBuffer\n");
	}

	ZStencilBuffer::~ZStencilBuffer()
	{
		delete m_uniform;

		delete m_outlineDescLayout;

		vkDestroyPipeline(ZDevice::getDevice(), m_stencil, nullptr);
		vkDestroyPipeline(ZDevice::getDevice(), m_outline, nullptr);

		vkDestroyPipelineLayout(ZDevice::getDevice(), m_stencilLayout , nullptr);
		vkDestroyPipelineLayout(ZDevice::getDevice(), m_outlineLayout, nullptr);

		vkDestroyShaderModule(ZDevice::getDevice(), m_outlineVertModule, nullptr);
		vkDestroyShaderModule(ZDevice::getDevice(), m_outlineFragModule, nullptr);
		
		printf("destroyed ZStencilBuffer\n");
	}

    void createPipelineLayout(VkPipelineLayout pipelineLayout, VkDescriptorSetLayout* descLayouts, uint32_t descCount) {
        VkPipelineLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pSetLayouts = descLayouts;
        layoutCreateInfo.setLayoutCount = descCount;
        layoutCreateInfo.pushConstantRangeCount = 0;
        layoutCreateInfo.pPushConstantRanges = nullptr;

        VkResult res = vkCreatePipelineLayout(ZDevice::getDevice(), &layoutCreateInfo, nullptr, &pipelineLayout);
        if (res != VK_SUCCESS) 
            printf("create pipelineLayout ERROR: %d\n", res);
    }
    
	void ZStencilBuffer::createPipelines(ZVertexLayout* vertexLayout, VkShaderModule& shaderModuleVert, VkShaderModule& shaderModuleFrag, VkDescriptorSetLayout* descLayouts, uint32_t layoutsCount)
	{
		createPipelineLayout(m_stencilLayout, descLayouts, layoutsCount);
		createPipelineLayout(m_outlineLayout, &m_outlineDescLayout->layout, 1);

		VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert;
		shaderStageCreateInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageCreateInfoVert.pNext = nullptr;
		shaderStageCreateInfoVert.flags = 0;
		shaderStageCreateInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageCreateInfoVert.module = shaderModuleVert;
		shaderStageCreateInfoVert.pName = "main";
		shaderStageCreateInfoVert.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag;
		shaderStageCreateInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageCreateInfoFrag.pNext = nullptr;
		shaderStageCreateInfoFrag.flags = 0;
		shaderStageCreateInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStageCreateInfoFrag.module = shaderModuleFrag;
		shaderStageCreateInfoFrag.pName = "main";
		shaderStageCreateInfoFrag.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo shaderStages[] = { shaderStageCreateInfoVert, shaderStageCreateInfoFrag };

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.pNext = nullptr;
		vertexInputStateCreateInfo.flags = 0;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexLayout->bindingDescritption;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexLayout->vertexAttributeDesc.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexLayout->vertexAttributeDesc.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.pNext = nullptr;
		inputAssemblyStateCreateInfo.flags = 0;
		inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCreateInfo.primitiveRestartEnable = false;

        VkViewport viewport;
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = (float) 800;         // TODO: parametrize
		viewport.height = (float) 600;        // TODO: parametrize
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		VkRect2D scissor;
		scissor.offset = { 0, 0 };
		scissor.extent = { 800, 600 };          // TODO: parametrize


		VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.pNext = nullptr;
		viewportStateCreateInfo.flags = 0;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.pNext = nullptr;
		rasterizationStateCreateInfo.flags = 0;
		rasterizationStateCreateInfo.depthClampEnable = false;
		rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationStateCreateInfo.depthBiasEnable = false;
		rasterizationStateCreateInfo.depthBiasConstantFactor = 0.f;
		rasterizationStateCreateInfo.depthBiasClamp = 0.f;
		rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.f;
		rasterizationStateCreateInfo.lineWidth = 1.f;

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

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stageCount = 2;
		pipelineCreateInfo.pStages = shaderStages;
		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
		pipelineCreateInfo.pTessellationState = nullptr;
		pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
		pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
		pipelineCreateInfo.pDepthStencilState = &depthStencil;
		pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
		pipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
		pipelineCreateInfo.layout = m_stencilLayout;
		pipelineCreateInfo.renderPass = ZDevice::getRenderPass();
		pipelineCreateInfo.subpass = 0;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		// ----------------------------
		// stencil fill pass
		// ----------------------------
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = true;
		depthStencil.depthWriteEnable = true;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencil.depthBoundsTestEnable = false;
		depthStencil.stencilTestEnable = true;

		depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;
		depthStencil.back.failOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.depthFailOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.passOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.compareMask = 0xff;
		depthStencil.back.writeMask = 0xff;
		depthStencil.back.reference = 1;
		depthStencil.front = depthStencil.back;

		VkResult res = vkCreateGraphicsPipelines(ZDevice::getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_stencil);
        if (res != VK_SUCCESS)
            printf("create graphic pipeline ERROR: %d\n", res);
		// ----------------------------

		// ----------------------------
		// outline pass
		// ----------------------------
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_LINE;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;

		pipelineCreateInfo.layout = m_outlineLayout;

		depthStencil.depthTestEnable = false;
		depthStencil.back.compareOp = VK_COMPARE_OP_NOT_EQUAL;
		depthStencil.back.failOp = VK_STENCIL_OP_KEEP;
		depthStencil.back.depthFailOp = VK_STENCIL_OP_KEEP;
		depthStencil.back.passOp = VK_STENCIL_OP_REPLACE;
		depthStencil.front = depthStencil.back;

		shaderStages[0].module = m_outlineVertModule;
		shaderStages[1].module = m_outlineFragModule;

		res = vkCreateGraphicsPipelines(ZDevice::getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_outline);
        if (res != VK_SUCCESS)
            printf("create graphic pipeline ERROR: %d\n", res);
		// ----------------------------
	}

	void ZStencilBuffer::createUniform()
	{
		m_uniform = new ZUniform();
        m_uniform->addComponent(ZType::MAT4);
        m_uniform->addComponent(ZType::MAT4);
        m_uniform->addComponent(ZType::MAT4);
		
		m_uniform->create();
	}

	void ZStencilBuffer::createDescSet(const VkDescriptorPool& descriptorPool)
	{
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = descriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = &m_outlineDescLayout->layout;

		VkResult res = vkAllocateDescriptorSets(ZDevice::getDevice(), &descriptorSetAllocateInfo, &m_outlineDescSet);
        if (res != VK_SUCCESS)
            printf("create graphic pipeline ERROR: %d\n", res);

		VkWriteDescriptorSet writeDescriptorSets = {};
		writeDescriptorSets.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets.dstSet = m_outlineDescSet;
		writeDescriptorSets.dstBinding = 0;
		writeDescriptorSets.dstArrayElement = 0;
		writeDescriptorSets.descriptorCount = 1;
		writeDescriptorSets.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets.pBufferInfo = m_uniform->getBufferInfo();

		vkUpdateDescriptorSets(ZDevice::getDevice(), 1, &writeDescriptorSets, 0, nullptr);
	}
}
