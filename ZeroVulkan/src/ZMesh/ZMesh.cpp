#include "ZMesh.h"
#include <vulkan/vulkan_core.h>

namespace ZeroVulkan {
    ZMesh::ZMesh() {
        vertices = {
              0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
              0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
             -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
             -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
            
              0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
              0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
        };
        
        indices = {
            0, 1, 2,
            2, 3, 0,

            5, 1, 0,
            0, 4, 5,

            6, 3, 2,
            6, 7, 3,

            5, 2, 1,
            5, 6, 2,

            0, 3, 4,
            7, 4, 3,

            4, 7, 6,
            6, 5, 4
        };
        indices_count = indices.size();

        
        createAndUploadBuffer(ZDevice::getDevice(), vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBuffer, m_vertexMemory);
        createAndUploadBuffer(ZDevice::getDevice(), indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer, m_indexMemory);
    }

    void ZMesh::bind(VkCommandBuffer& cmdBuffer) {
        VkDeviceSize offsets[] = { 0 };
        
        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, offsets);
        vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmdBuffer, indices_count, 1, 0, 0, 0);
    }

    ZMesh::~ZMesh() {
        vkDestroyBuffer(ZDevice::getDevice(), vertexBuffer, nullptr);       
        vkFreeMemory(ZDevice::getDevice(), m_vertexMemory, nullptr);

        vkDestroyBuffer(ZDevice::getDevice(), indexBuffer, nullptr);       
        vkFreeMemory(ZDevice::getDevice(), m_indexMemory, nullptr);
    }
}
