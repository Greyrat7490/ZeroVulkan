#include "ZObject.h"
#include "ZRenderer/ZRenderer.h"
#include "ZShaders/ZShaders.h"
#include "ZScene/ZScene.h"
#include <functional>

namespace ZeroVulkan {
    ZObject::ZObject(ZShaders& shaders, ZMesh& mesh)
        : shaders(std::move(shaders)), mesh(std::move(mesh))
    {}
    
    ZObject::ZObject()
        : shaders( ZShaders("Test/shader/compiled/phong.vert.spv", "Test/shader/compiled/phong.frag.spv") ) 
    {}
    
    void ZObject::update(float dt) {}
        
    void ZObject::addBindsToScene(ZScene* scene) {
        // order is important
        scene->addBindFunction(std::bind(&ZShaders::bind, &shaders, std::placeholders::_1 ));
        scene->addBindFunction(std::bind(&ZMesh::bind, &mesh, std::placeholders::_1 ));
    }    
}
