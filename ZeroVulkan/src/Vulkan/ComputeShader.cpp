#include "ComputeShader.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Buffer.h"
#include "utils.h"

namespace ZeroVulkan
{
    std::vector<ZComputeShader*> ZComputeShader::computeShaders;

    ZComputeShader::ZComputeShader(uint32_t count)
        : count(count)
    {
        computeShaders.push_back(this);

        commandPool = new ZCommandPool();
        commandPool->createCommandBuffers(1);

        descriptorPool = new ZDescriptorPool();
        descriptorSet = new ZDescriptorSet();
        descriptorSetLayout = new ZDescriptorSetLayout();

        printf("created: ZComputeShader\n");
    }

    ZComputeShader::~ZComputeShader()
    {
        vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
        vkDestroyPipelineLayout(ZDevice::getDevice(), pipelineLayout, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModule, nullptr);
        vkDestroyBuffer(ZDevice::getDevice(), storageBuffer->storageBuffer, nullptr);
        vkFreeMemory(ZDevice::getDevice(), storageBuffer->storageMemory, nullptr);

        delete commandPool;
        delete descriptorPool;
        delete descriptorSet;
        delete descriptorSetLayout;
        delete uniformBuffer;
        delete storageBuffer;

        printf("destroyed ZComputeShader\n");
    }

    void ZComputeShader::create()
    {
        VkPipelineLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pSetLayouts = &descriptorSetLayout->layout;
        layoutCreateInfo.setLayoutCount = 1;
        layoutCreateInfo.pushConstantRangeCount = 0;
        layoutCreateInfo.pPushConstantRanges = nullptr;

        VkResult res = vkCreatePipelineLayout(ZDevice::getDevice(), &layoutCreateInfo, nullptr, &pipelineLayout);
        if (res != VK_SUCCESS) 
            printf("create pipelineLayout ERROR: %d\n", res);

        // TODO parameterize
        createShaderModule("Test/shader/compiled/particle.comp.spv", &shaderModule);

        createComputePipeline(pipeline, pipelineLayout, shaderModule);
    }

    void ZComputeShader::buildCommandBuffer()
    {
        VkCommandBufferBeginInfo cmdBufferInfo = {};
        cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VkResult res = vkBeginCommandBuffer(commandPool->getBuffers()[0], &cmdBufferInfo);
        if (res != VK_SUCCESS)
            printf("begin command ERROR: %d\n", res);

        vkCmdBindPipeline(commandPool->getBuffers()[0], VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(commandPool->getBuffers()[0], VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet->descSet, 0, 0);
        vkCmdDispatch(commandPool->getBuffers()[0], count, 1, 1);

        vkEndCommandBuffer(commandPool->getBuffers()[0]);
    }
    
    void ZComputeShader::submit(VkQueue queue)
    {
        VkSubmitInfo computeSubmitInfo = {};
        computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        computeSubmitInfo.commandBufferCount = 1;
        computeSubmitInfo.pCommandBuffers = commandPool->getBuffers().data();
        computeSubmitInfo.waitSemaphoreCount = 0;
        computeSubmitInfo.pWaitSemaphores = nullptr;
        computeSubmitInfo.pWaitDstStageMask = nullptr;
        computeSubmitInfo.signalSemaphoreCount = 0;
        computeSubmitInfo.pSignalSemaphores = nullptr;
        
        VkResult res = vkQueueSubmit(queue, 1, &computeSubmitInfo, VK_NULL_HANDLE);
        if (res != VK_SUCCESS)
            printf("queue submit ERROR %d\n", res);
    }
}
