#include "ZObject.h"
#include "ZScene/ZScene.h"
#include <functional>
#include <assert.h>

namespace ZeroVulkan {
    ZObject::ZObject(ZShaderSet& shaders, ZMesh& mesh)
        : shaders(std::move(shaders)), mesh(std::move(mesh))
    {}
    
    ZObject::ZObject()
        : shaders( ZShaderSet("Test/shader/compiled/phong.vert.spv", "Test/shader/compiled/phong.frag.spv") ) 
    {}
    
    void ZObject::update(float dt) {
        (void)dt;

        assert(m_proj && m_view);
        
        *m_proj = m_scene->getProjection();
        *m_view = m_scene->getView();
        shaders.update();
    }
        
    void ZObject::addToScene(ZScene* scene) {
        // order is important
        scene->addBindFunction(std::bind(&ZShaderSet::bind, &shaders, std::placeholders::_1 ));
        scene->addBindFunction(std::bind(&ZMesh::bind, &mesh, std::placeholders::_1 ));
        m_scene = scene;
    }    
}
