#ifndef ZV_OBJECT_H_
#define ZV_OBJECT_H_

#include <vector>
#include "Core.h"
#include "Material.h"

namespace ZeroVulkan {
    class ZObject {
    public:
        ZObject();        
 
        void update(float dt);

        ZMaterial material;
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

#endif // ifndef ZV_OBJECT_H_
