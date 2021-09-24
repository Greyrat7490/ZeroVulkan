#ifndef ZMESH_H_
#define ZMESH_H_

#include "Vulkan/UniformBuffer.h"

namespace ZeroVulkan {
    class ZMesh {
        public:
            ZMesh();
            ~ZMesh();

            void bind(VkCommandBuffer& cmdBuffer);
            VkBuffer vertexBuffer = nullptr;
            VkBuffer indexBuffer = nullptr;
            uint32_t indices_count = 0;
            ZUniform uniform;

        private:
            VkDeviceMemory m_vertexMemory = nullptr;
            VkDeviceMemory m_indexMemory = nullptr;
            std::vector<float> vertices;
            std::vector<uint32_t> indices;
    };
}

#endif // ZMESH_H_
