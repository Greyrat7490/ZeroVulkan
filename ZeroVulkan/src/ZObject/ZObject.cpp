#include "ZObject.h"
#include "ZScene/ZScene.h"

namespace ZeroVulkan {
    ZObject::ZObject(ZShaderSet& shaders, ZMesh& mesh)
        : m_shaders(std::move(shaders)), m_mesh(std::move(mesh))
    {}

    ZObject::ZObject(ZMesh& mesh)
        : m_shaders( ZShaderSet("Test/shader/phong.vert", "Test/shader/phong.frag") ), m_mesh(std::move(mesh))
    {
        m_proj = m_shaders.getUniform().getComponent<mat4>(0);
        m_view = m_shaders.getUniform().getComponent<mat4>(1);
        m_model = m_shaders.getUniform().getComponent<mat4>(2);
        *m_model = mat4(1.f);

        vec3* lightDir = m_shaders.getUniform().getComponent<vec3>(3);
        *lightDir = vec3(-1.f, -1.f, 3.f);
    }

    void ZObject::update(ZScene* scene) {
        ZASSERT_FUNC(m_proj && m_view, "matrices for 3D rendering are not provided");

        *m_proj = scene->getProjection();
        *m_view = scene->getView();
    }

    void ZObject::set3DMats(mat4* proj, mat4* view, mat4* model) {
        m_proj = proj;
        m_view = view;
        m_model = model;
    }

    void ZObject::bind(VkCommandBuffer& cmdBuffer) {
        m_mesh.bind(cmdBuffer);
        m_shaders.bind(cmdBuffer);
        m_mesh.drawCmd(cmdBuffer);

        if (m_shaders.bindStencil(cmdBuffer))
            m_mesh.drawCmd(cmdBuffer);
    }
}
