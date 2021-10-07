#ifndef ZMESH_H_
#define ZMESH_H_

#include "Vulkan/Buffer.h"

namespace ZeroVulkan {
    class ZMesh {
        public:
            ZMesh() = default;
            ZMesh(float* vertices, uint32_t vertices_count, uint32_t* indices, uint32_t indices_count);
            ~ZMesh();
            ZMesh(ZMesh&& source);
            ZMesh& operator=(ZMesh&& source);

            void setVertices(float* vertices, uint32_t vertices_count);
            void setIndices(uint32_t* indices, uint32_t indices_count);
            
            void bind(VkCommandBuffer& cmdBuffer);
            void prepair();
            
            VkBuffer vertexBuffer = nullptr;
            VkBuffer indexBuffer = nullptr;

        private:
            VkDeviceMemory m_vertexMemory = nullptr;
            VkDeviceMemory m_indexMemory = nullptr;
            std::vector<float> m_vertices;
            std::vector<uint32_t> m_indices;

            bool ready = false;
    };
}

#endif // ZMESH_H_
