#ifndef ZOBJECT_H_
#define ZOBJECT_H_

#include "ZShaderSet/ZShaderSet.h"
#include "ZMesh/ZMesh.h"
#include <cstddef>

namespace ZeroVulkan {
    class ZScene;
    class ZUniform;
    
    class ZObject {
    public:
        ZObject(ZMesh& mesh);
        ZObject(ZShaderSet& shaders, ZMesh& mesh);
 
        void update(float dt);

        void addToScene(ZScene* scene);

        inline ZUniform& getUniform(size_t index = 0) { return m_shaders.getUniform(index); }
        template<typename T>
        inline T* getUniformComponent(size_t index, size_t uniformIdx = 0) { return m_shaders.getUniform(uniformIdx).getComponent<T>(index); }
        
        void setShaders(const ZShaderSet& shaders);
        void set3DMats(mat4* proj, mat4* view, mat4* model);
    private:
        ZShaderSet m_shaders;
        ZMesh m_mesh;
        ZScene* m_scene;
        
        // for 3D
        mat4* m_proj;
        mat4* m_view;
        mat4* m_model;
    };
}

#endif // ZOBJECT_H_
