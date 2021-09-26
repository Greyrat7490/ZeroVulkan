#include "ZObject.h"
#include "ZRenderer/ZRenderer.h"
#include "ZShaders/ZShaders.h"

namespace ZeroVulkan {
    void ZObject::update(float dt) {
    }

    ZObject::ZObject(ZScene* attachedScene)
        : shaders( ZShaders("Test/shader/compiled/phong.vert.spv", "Test/shader/compiled/phong.frag.spv") ), attachedScene(attachedScene)
    {
        attachedScene->addBindFunction( std::bind( &ZShaders::bind, &shaders, std::placeholders::_1 ) );
        attachedScene->addBindFunction( std::bind( &ZMesh::bind, &mesh, std::placeholders::_1 ) );
    }
}
