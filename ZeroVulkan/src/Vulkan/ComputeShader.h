#ifndef H_COMPUTER_SHADER
#define H_COMPUTER_SHADER

#include <vector>
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "CommandBuffer.h"
#include "Uniform.h"
#include "StorageBuffer.h"

namespace ZeroVulkan
{
	class ZComputeShader
	{
	public:
		ZComputeShader(const char* path, uint32_t count);
		~ZComputeShader();

		void create();

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

        size_t count = 0;
		uint32_t storageComponentsCount = 0;

		VkPipeline pipeline = nullptr;
		VkPipelineLayout pipelineLayout = nullptr;
		VkShaderModule shaderModule = nullptr;
	};
}

#endif // H_COMPUTER_SHADER
