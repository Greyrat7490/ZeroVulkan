#ifndef H_COMPUTER_SHADER
#define H_COMPUTER_SHADER

#include <vector>
#include "Device.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "CommandBuffer.h"
#include "Uniform.h"

namespace ZeroVulkan
{
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
		ZComputeShader(uint32_t count);
		~ZComputeShader();

		void create();

		template<typename... Layout>
		void createStorageBuffer(Layout&&... components);
		template<typename T>
		void setStorageBuffer(T* buffer);
		void buildCommandBuffer();
		void submit(VkQueue queue);

		ZCommandPool* commandPool;

		uint32_t count = 0;
		uint32_t storageComponentsCount = 0;

		ZDescriptorPool* descriptorPool = nullptr;
		ZDescriptorSet* descriptorSet = nullptr;
		ZDescriptorSetLayout* descriptorSetLayout = nullptr;

		ZUniform* uniformBuffer = nullptr;
		ZStorageBuffer* storageBuffer = nullptr;

		VkPipeline pipeline = nullptr;
		VkPipelineLayout pipelineLayout = nullptr;
		VkShaderModule shaderModule = nullptr;

		static std::vector<ZComputeShader*> computeShaders;
	};

	// watch later why linker causes error if in source file
	template<typename ...Layout>
	inline void ZComputeShader::createStorageBuffer(Layout&& ...components)
	{
		storageBuffer = new ZStorageBuffer;

		storageComponentsCount = sizeof...(components);

		auto layoutArray = { components... };

		uint32_t i = 0;
		std::for_each(layoutArray.begin(), layoutArray.end(), [&](auto component) {
			storageBuffer->storageSize += static_cast<VkDeviceSize>(component);
		} );

		createBuffer(
			ZDevice::getDevice(),
			storageBuffer->storageSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			storageBuffer->storageBuffer,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			storageBuffer->storageMemory
		);

		storageBuffer->bufferInfo = new VkDescriptorBufferInfo;
		storageBuffer->bufferInfo->offset = 0;
		storageBuffer->bufferInfo->range = storageBuffer->storageSize;
		storageBuffer->bufferInfo->buffer = storageBuffer->storageBuffer;

		printf("storageSize: %d\n", storageBuffer->storageSize);

		if(storageBuffer->storageSize % 16)
			printf("storageSize is no a multiple of 16 (see glsl alignment requirements)\n");
	}
	
	template<typename T>
	inline void ZComputeShader::setStorageBuffer(T* buffer)
	{
		VkBuffer staggingBuffer;
		VkDeviceMemory memory;

		createBuffer(
			ZDevice::getDevice(),
			storageBuffer->storageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			staggingBuffer,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			memory
		);

		void* mappedData;
		vkMapMemory(ZDevice::getDevice(), memory, 0, storageBuffer->storageSize, 0, &mappedData);
		
		memcpy(mappedData, buffer, storageBuffer->storageSize);

		copyBuffer(staggingBuffer, storageBuffer->storageBuffer, storageBuffer->storageSize);

		vkUnmapMemory(ZDevice::getDevice(), memory);
		vkFreeMemory(ZDevice::getDevice(), memory, nullptr);
		vkDestroyBuffer(ZDevice::getDevice(), staggingBuffer, nullptr);
	}
}

#endif // H_COMPUTER_SHADER
