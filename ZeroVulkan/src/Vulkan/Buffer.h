#ifndef H_BUFFER
#define H_BUFFER

#include <stdio.h>
#include <cstring>
#include "Device.h"
#include "Memory.h"
#include "CommandBuffer.h"

namespace ZeroVulkan
{
    inline void createBuffer(VkDevice dev, VkDeviceSize bufferSize,
        VkBufferUsageFlags bufferUsageFlag,
        VkBuffer& buffer,
        VkMemoryPropertyFlags memPropertyFlags,
        VkDeviceMemory& devBufferMem)
    {
        VkBufferCreateInfo bufferCreateInfo;
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.pNext = nullptr;
        bufferCreateInfo.flags = 0;
        bufferCreateInfo.size = bufferSize;
        bufferCreateInfo.usage = bufferUsageFlag;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferCreateInfo.queueFamilyIndexCount = 0;
        bufferCreateInfo.pQueueFamilyIndices = nullptr;

        VkResult res = vkCreateBuffer(dev, &bufferCreateInfo, nullptr, &buffer);
        if (res != VK_SUCCESS)
            printf("CreateBuffer ERROR: %d\n", (int)res);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(dev, buffer, &memRequirements);

        VkMemoryAllocateInfo memAllocInfo;
        memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAllocInfo.pNext = nullptr;
        memAllocInfo.allocationSize = memRequirements.size;
        memAllocInfo.memoryTypeIndex = findMemTypeIndex(ZDevice::getPhysicalDev()[0], memRequirements.memoryTypeBits, memPropertyFlags);

        res = vkAllocateMemory(dev, &memAllocInfo, nullptr, &devBufferMem);
        if (res != VK_SUCCESS)
            printf("Allocate Memory ERROR: %d\n", res);

        vkBindBufferMemory(dev, buffer, devBufferMem, 0);
    }

    inline void copyBuffer(VkBuffer src, VkBuffer dest, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = ZDevice::getCommandPool()->beginCommand();

        VkBufferCopy bufferCopy;
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = size;

        vkCmdCopyBuffer(commandBuffer, src, dest, 1, &bufferCopy);

        ZDevice::getCommandPool()->endCommand(ZDevice::getQueue(), commandBuffer);
    }

    template<typename T>
    inline void createAndUploadBuffer(VkDevice dev, std::vector<T> data, VkBufferUsageFlags usage, VkBuffer& buffer, VkDeviceMemory& devMem)
    {
        VkDeviceSize devSize = sizeof(data[0]) * data.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMem;
        createBuffer(dev, devSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBufferMem);

        void* rawData;
        vkMapMemory(dev, stagingBufferMem, 0, devSize, 0, &rawData);
        memcpy(rawData, data.data(), devSize);
        vkUnmapMemory(dev, stagingBufferMem);

        createBuffer(dev, devSize, usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, devMem);

        copyBuffer(stagingBuffer, buffer, devSize);

        vkDestroyBuffer(dev, stagingBuffer, nullptr);
        vkFreeMemory(dev, stagingBufferMem, nullptr);
    }

	inline void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int offsetX = 0, int offsetY = 0) {
		VkCommandBuffer commandBuffer = ZDevice::getCommandPool()->beginCommand();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { offsetX, offsetY, 0 };
		region.imageExtent = { width, height, 1	};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

        ZDevice::getCommandPool()->endCommand(ZDevice::getQueue(), commandBuffer);
	}
}

#endif // H_BUFFER
