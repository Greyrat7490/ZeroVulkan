#ifndef H_COMPUTER_SHADER
#define H_COMPUTER_SHADER

#include <vector>
#include <vulkan/vulkan_core.h>
#include "Device.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "CommandBuffer.h"
#include "Uniform.h"

namespace ZeroVulkan
{
    // TODO: create a buffer class
	struct ZStorageBuffer
	{
		void* mappedData = nullptr;
		VkBuffer storageBuffer = nullptr;
		VkDeviceMemory storageMemory = nullptr;
		uint32_t storageSize = 0;
		VkDescriptorBufferInfo* bufferInfo = nullptr;
	};

	class ZComputeShader
	{
	public:
		ZComputeShader(const char* path, uint32_t count);
		~ZComputeShader();

		void create();

		void createStorageBuffer(size_t size);
		void setStorageBuffer(void* buffer);
		void buildCommandBuffer();
		void submit(VkQueue queue);

        void bind(VkCommandBuffer& cmdBuffer);

        // TODO: remove this and use the descriptorPool of ZShaderSet
		ZDescriptorPool descriptorPool;
		ZDescriptorSet descriptorSet;
		ZDescriptorSetLayout descriptorSetLayout;

		ZUniform uniformBuffer;
		ZStorageBuffer storageBuffer;
    private:
		ZCommandPool commandPool;

        uint32_t count = 0;
		uint32_t storageComponentsCount = 0;

		VkPipeline pipeline = nullptr;
		VkPipelineLayout pipelineLayout = nullptr;
		VkShaderModule shaderModule = nullptr;

        void parseUniforms(const std::string& file);
        void parseStorageBuffers(const std::string& file);
	};
}

#endif // H_COMPUTER_SHADER
