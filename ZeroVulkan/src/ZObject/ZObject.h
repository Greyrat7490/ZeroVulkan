#ifndef ZOBJECT_H_
#define ZOBJECT_H_

#include "ZShaderSet/ZShaderSet.h"
#include "ZMesh/ZMesh.h"

namespace ZeroVulkan {
    class ZScene;
    
    class ZObject {
    public:
        ZObject();
        ZObject(ZShaderSet& shaders, ZMesh& mesh);
 
        void update(float dt);

        void addToScene(ZScene* scene);
        void set3DMats(mat4* proj, mat4* view) { m_proj = proj; m_view = view; }
        void setShaders(const ZShaderSet& shaders); 
        void setMesh(const ZMesh& mesh); 
    private:
        ZShaderSet shaders;
        ZMesh mesh;
        ZScene* m_scene;
        
        // for 3D
        mat4* m_proj;
        mat4* m_view;
    };
}

#endif // ZOBJECT_H_
