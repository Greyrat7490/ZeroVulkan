#include "ZMesh.h"

#include <cstring>
#include <cstdio>

#include "Vulkan/Device.h"
#include "Vulkan/Buffer.h"


namespace ZeroVulkan {
    ZMesh::ZMesh(float* vertices, uint32_t vertices_count, uint32_t* indices, uint32_t indices_count) {
        setVertices(vertices, vertices_count);
        setIndices(indices, indices_count);
    }
    
    ZMesh::ZMesh(ZMesh&& source) {
        m_vertices.swap(source.m_vertices);
        m_indices.swap(source.m_indices);
        ready = false;        
        
        puts("moved ZMesh");
    }

    ZMesh& ZMesh::operator=(ZMesh&& source) {
        m_vertices.swap(source.m_vertices);
        m_indices.swap(source.m_indices);
        ready = false;        
        
        puts("moved ZMesh (=)");

        return *this;
    }

    void ZMesh::setVertices(float* vertices, uint32_t vertices_count) {
        m_vertices.resize(vertices_count);
        memcpy(m_vertices.data(), vertices, vertices_count * sizeof(float));
    }
    
    void ZMesh::setIndices(uint32_t* indices, uint32_t indices_count) {
        m_indices.resize(indices_count);
        memcpy(m_indices.data(), indices, indices_count * sizeof(uint32_t));
    }
    
    void ZMesh::prepair() {
        createAndUploadBuffer(ZDevice::getDevice(), m_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBuffer, m_vertexMemory);
        createAndUploadBuffer(ZDevice::getDevice(), m_indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer, m_indexMemory);
    }
    
    void ZMesh::bind(VkCommandBuffer& cmdBuffer) {
        if (!ready) {
            prepair();
            ready = true;
        }
            
        VkDeviceSize offsets[] = { 0 };
        
        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, offsets);
        vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    ZMesh::~ZMesh() {
        if (ready) {
            vkDestroyBuffer(ZDevice::getDevice(), vertexBuffer, nullptr);       
            vkFreeMemory(ZDevice::getDevice(), m_vertexMemory, nullptr);

            vkDestroyBuffer(ZDevice::getDevice(), indexBuffer, nullptr);       
            vkFreeMemory(ZDevice::getDevice(), m_indexMemory, nullptr);
        }
    }
}
