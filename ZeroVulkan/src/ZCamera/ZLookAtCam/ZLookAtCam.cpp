#include "ZLookAtCam.h"

namespace ZeroVulkan
{
    ZLookAtCam::ZLookAtCam() {
        lookAt(vec3(0.f));
    }

    void ZLookAtCam::updateView()
    {
        m_direction = normal(m_pos - m_lookAtPos);
        vec3 right = normal(cross(vec3(0.f, 0.f, 1.f), m_direction));
        vec3 up = cross(m_direction, right);

        m_rot[0] = std::acos(right[0]);
        m_rot[1] = std::acos(up[2]);

        m_view[0] = vec4(right[0], up[0], m_direction[0], 0);
        m_view[1] = vec4(right[1], up[1], m_direction[1], 0);
        m_view[2] = vec4(right[2], up[2], m_direction[2], 0);
        m_view[3] = vec4(-dot(right, m_pos), -dot(up, m_pos), -dot(m_direction, m_pos), 1);
    }

    void ZLookAtCam::lookAt(vec3 pos)
    {
        m_lookAtPos = pos;
        updateView();
    }
}
