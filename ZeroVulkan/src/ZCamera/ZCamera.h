#ifndef ZCAMERA_H_
#define ZCAMERA_H_

#include "types.h"
#include <cstdint>

namespace ZeroVulkan
{
    class ZCamera {
    public:
        ZCamera();
        ZCamera(uint32_t width, uint32_t height);

        virtual void updateView() = 0;

        const mat4& getView() const { return m_view; }
        const mat4& getProj() const { return m_proj; }

        void setPos(vec3 pos);
        vec3 getPos() const { return m_pos; }

        void updateProj();
        void setAspect();
        void setAspect(uint32_t width, uint32_t height);
    protected:
        vec3 m_pos;
        vec2 m_size;

        float m_aspect = 1.77777f;
        float m_fovY = 80.f;
        float m_farValue = 30.f;
        float m_nearValue = 0.001f;

        mat4 m_proj;
        mat4 m_view;
    };
}

#endif // ZCAMERA_H_
