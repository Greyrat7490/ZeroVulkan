#ifndef ZMESH_H_
#define ZMESH_H_

#include "Vulkan/Buffer.h"

namespace ZeroVulkan {
    class ZMesh {
        public:
            ZMesh();
            ~ZMesh();
            ZMesh(ZMesh&& source);
            ZMesh& operator=(ZMesh&& source);

            void bind(VkCommandBuffer& cmdBuffer);
            void prepair();
            
            VkBuffer vertexBuffer = nullptr;
            VkBuffer indexBuffer = nullptr;

        private:
            VkDeviceMemory m_vertexMemory = nullptr;
            VkDeviceMemory m_indexMemory = nullptr;
            std::vector<float> vertices;
            std::vector<uint32_t> indices;

            bool ready = false;
    };
}

#endif // ZMESH_H_
