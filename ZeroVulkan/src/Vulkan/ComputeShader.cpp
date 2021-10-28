#include "ComputeShader.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Buffer.h"
#include "utils.h"

namespace ZeroVulkan
{
    ZComputeShader::ZComputeShader(const char* path, uint32_t count)
        : count(count)
    {
        commandPool.createCommandBuffers(1);

        std::string absPath = pathToAbsolue(path);
        createShaderModule(absPath, &shaderModule);

        std::string file = readFile(absPath);
        trimFile(file);
        parseUniforms(file);
        // parseStorageBuffers(file);

        printf("created: ZComputeShader\n");
    }

    ZComputeShader::~ZComputeShader()
    {
        vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
        vkDestroyPipelineLayout(ZDevice::getDevice(), pipelineLayout, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModule, nullptr);
        vkDestroyBuffer(ZDevice::getDevice(), storageBuffer.storageBuffer, nullptr);
        vkFreeMemory(ZDevice::getDevice(), storageBuffer.storageMemory, nullptr);

        printf("destroyed ZComputeShader\n");
    }

    void ZComputeShader::create()
    {
        VkPipelineLayoutCreateInfo layoutCreateInfo = {};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pSetLayouts = &descriptorSetLayout.layout;
        layoutCreateInfo.setLayoutCount = 1;
        layoutCreateInfo.pushConstantRangeCount = 0;
        layoutCreateInfo.pPushConstantRanges = nullptr;

        VkResult res = vkCreatePipelineLayout(ZDevice::getDevice(), &layoutCreateInfo, nullptr, &pipelineLayout);
        if (res != VK_SUCCESS) 
            printf("create pipelineLayout ERROR: %d\n", res);

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

        res = vkCreateComputePipelines(ZDevice::getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);
        if (res != VK_SUCCESS)
            printf("create computer pipline ERROR: %d\n", res); 
    }

    void ZComputeShader::buildCommandBuffer()
    {
        VkCommandBufferBeginInfo cmdBufferInfo = {};
        cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VkResult res = vkBeginCommandBuffer(commandPool.getBuffers()[0], &cmdBufferInfo);
        if (res != VK_SUCCESS)
            printf("begin command ERROR: %d\n", res);

        vkCmdBindPipeline(commandPool.getBuffers()[0], VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(commandPool.getBuffers()[0], VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet.descSet, 0, 0);
        vkCmdDispatch(commandPool.getBuffers()[0], count, 1, 1);

        printf("dispatched %u compute shaders\n", count);
        
        vkEndCommandBuffer(commandPool.getBuffers()[0]);
    }

	void ZComputeShader::createStorageBuffer(size_t size)
	{
        storageBuffer.storageSize = size;

		createBuffer(
			ZDevice::getDevice(),
			storageBuffer.storageSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			storageBuffer.storageBuffer,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			storageBuffer.storageMemory
		);

		storageBuffer.bufferInfo = new VkDescriptorBufferInfo;
		storageBuffer.bufferInfo->offset = 0;
		storageBuffer.bufferInfo->range = storageBuffer.storageSize;
		storageBuffer.bufferInfo->buffer = storageBuffer.storageBuffer;

		printf("storageSize: %d\n", storageBuffer.storageSize);

		if(storageBuffer.storageSize % 16)
			printf("storageSize is not a multiple of 16 (see glsl alignment requirements)\n");
	}
    
	void ZComputeShader::setStorageBuffer(void* buffer)
	{
		VkBuffer staggingBuffer;
		VkDeviceMemory memory;

		createBuffer(
			ZDevice::getDevice(),
			storageBuffer.storageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			staggingBuffer,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			memory
		);

		void* mappedData;
		vkMapMemory(ZDevice::getDevice(), memory, 0, storageBuffer.storageSize, 0, &mappedData);
		
		memcpy(mappedData, buffer, storageBuffer.storageSize);

		copyBuffer(staggingBuffer, storageBuffer.storageBuffer, storageBuffer.storageSize);

		vkUnmapMemory(ZDevice::getDevice(), memory);
		vkFreeMemory(ZDevice::getDevice(), memory, nullptr);
		vkDestroyBuffer(ZDevice::getDevice(), staggingBuffer, nullptr);
	}

    // TODO: merge with parsser in ZShaderSet
    void ZComputeShader::parseUniforms(const std::string& file) {
        size_t bpos = file.find("binding");
        while (bpos != file.npos) {

            // get binding type -----------------------------
            size_t pos = file.find(")", bpos);
            if (pos == file.npos) {
                puts("the file is not correct");
                exit(1);
            }
            if (file.at(pos + 1) == ' ')
                pos += 2;
            std::string type = file.substr(pos, file.find(' ', pos) - pos);
            puts(type.c_str());
            // ----------------------------------------------

            if (type == "uniform") {

                // add binding ------------------------------
                std::string bindingStr = file.substr(file.find('=', bpos) + 1, file.find(')', bpos) - bpos);
                trimWord(bindingStr);
                size_t binding = std::stoll(bindingStr);
                
                descriptorSetLayout.addBinding(binding, uniformBuffer.getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
                
                printf("(binding = %zu) uniform\n", binding);
                // ------------------------------------------

                // add components ---------------------------
                pos = file.find('{', pos + 7) + 1;
                
                // skip whitespace at the beginning
                if (file.at(pos) == ' ')
                    pos++;
                
                std::string structDef = file.substr(pos, file.find('}', pos) - pos);
                
                pos = 0;
                size_t pos2 = structDef.find(' ', pos);
                while (pos2 != structDef.npos) {
                    std::string type = structDef.substr(pos, pos2 - pos);
                    
                    pos = structDef.find(';', pos) + 1;
                    if (structDef.at(pos) == ' ')
                        pos++;
                    pos2 = structDef.find(' ', pos);

                    if (uniformBuffer.addComponentByStr(type))
                        printf("  %s\n", type.c_str());
                }
                // ------------------------------------------
                
                uniformBuffer.create();
            }

            bpos = file.find("binding", bpos + 7);
        }
    }
    
    void ZComputeShader::parseStorageBuffers(const std::string& file) {
        (void) file;
        ZASSERT_FUNC(false, "not implemented yet");
    }
    
    void ZComputeShader::submit(VkQueue queue)
    {
        VkSubmitInfo computeSubmitInfo = {};
        computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        computeSubmitInfo.commandBufferCount = 1;
        computeSubmitInfo.pCommandBuffers = commandPool.getBuffers().data();
        computeSubmitInfo.waitSemaphoreCount = 0;
        computeSubmitInfo.pWaitSemaphores = nullptr;
        computeSubmitInfo.pWaitDstStageMask = nullptr;
        computeSubmitInfo.signalSemaphoreCount = 0;
        computeSubmitInfo.pSignalSemaphores = nullptr;
        
        VkResult res = vkQueueSubmit(queue, 1, &computeSubmitInfo, VK_NULL_HANDLE);
        if (res != VK_SUCCESS)
            printf("queue submit ERROR %d\n", res);
    }

    void ZComputeShader::bind(VkCommandBuffer& cmdBuffer) {
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &storageBuffer.storageBuffer, offsets);
        vkCmdDraw(cmdBuffer, count, 1, 0, 0);
    }
}
