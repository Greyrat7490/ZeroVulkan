#ifndef ZOBJECT_H_
#define ZOBJECT_H_

#include <initializer_list>
#include <random>
#include "types.h"

#include "ZShaderSet/ZShaderSet.h"
#include "ZMesh/ZMesh.h"


namespace ZeroVulkan {
    class ZScene;
    class ZUniform;
    
    class ZObject {
    public:
        ZObject(ZMesh& mesh);
        ZObject(ZShaderSet& shaders, ZMesh& mesh);

        void update(ZScene* scene);

        inline ZUniform& getUniform(size_t index = 0) { return m_shaders.getUniform(index); }
        template<typename T>
        inline T* getUniformComponent(size_t index, size_t uniformIdx = 0) {
            return m_shaders.getUniform(uniformIdx).getComponent<T>(index);
        }

        template<typename S>
        inline S* getUniformStruct(size_t uniformIdx, const std::initializer_list<ZType>& fields) {
            return m_shaders.getUniform(uniformIdx).getStruct<S>(fields);
        }

        template<typename S>
        inline S* getStencilUniformStruct(size_t uniformIdx, const std::initializer_list<ZType>& fields) {
            return m_shaders.getStencilUniform(uniformIdx).getStruct<S>(fields);
        }

        void setShaders(const ZShaderSet& shaders);
        void set3DMats(mat4* proj, mat4* view, mat4* model);

        void bind(VkCommandBuffer& cmdBuffer);
    private:
        ZShaderSet m_shaders;
        ZMesh m_mesh;

        // for 3D
        mat4* m_proj;
        mat4* m_view;
        mat4* m_model;
    };
}

#endif // ZOBJECT_H_
