#include "ZvObject.h"
#include "Device.h"

namespace ZeroVulkan {
    void ZObject::update(float dt) {
    }

    ZObject::ZObject()
        : material( ZMaterial("Test/shader/compiled/phong.vert.spv", "Test/shader/compiled/phong.frag.spv") )
    {
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
        
        material.vertexLayout->addLocation(0, ZType::VEC3, 0);
        material.vertexLayout->addLocation(1, ZType::VEC3, sizeof(vec3));
        material.vertexLayout->createBinding(2 * sizeof(vec3));

        material.descSetLayout->addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);

        ZUniformLayout uniformLayout = ZUniformLayout( {
            sizeof(mat4),
            sizeof(mat4),
            sizeof(mat4),
            sizeof(vec3),
        } );
        
        uniform.create(&uniformLayout);
        material.create(&uniform, false);
    }
}
