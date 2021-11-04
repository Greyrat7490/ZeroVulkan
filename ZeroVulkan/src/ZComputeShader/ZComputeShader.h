#ifndef ZCOMPUTER_SHADER_H_
#define ZCOMPUTER_SHADER_H_

#include <vector>

#include "Vulkan/DescriptorSetLayout.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptorSet.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/StorageBuffer.h"
#include "Vulkan/Uniform.h"


namespace ZeroVulkan
{
    class ZComputeShader
    {
    public:
        ZComputeShader(const char* path, uint32_t count);
        ~ZComputeShader();

        inline size_t getCount() { return count; }

        void create();

        void buildCommandBuffer();
        void submit(VkQueue queue);

        void bind(VkCommandBuffer& cmdBuffer);
        inline void drawCmd(VkCommandBuffer& cmdBuffer) const { vkCmdDraw(cmdBuffer, count, 1, 0, 0); }

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

#endif // ZCOMPUTER_SHADER_H_
