#include "ZLookAtCam.h"

namespace ZeroVulkan 
{
    ZLookAtCam::ZLookAtCam() {
        lookAt(vec3(0.f));
    }
    
    void ZLookAtCam::updateView() 
    {
        vec3 direction = normal(m_pos - m_lookAtPos);
        vec3 right = normal(cross(vec3(0.f, 0.f, 1.f), direction));
        vec3 up = cross(direction, right);

        m_view[0] = vec4(right[0], up[0], direction[0], 0);
        m_view[1] = vec4(right[1], up[1], direction[1], 0);
        m_view[2] = vec4(right[2], up[2], direction[2], 0);
        m_view[3] = vec4(-dot(right, m_pos), -dot(up, m_pos), -dot(direction, m_pos), 1);
    }

    void ZLookAtCam::lookAt(vec3 pos) 
    {
        m_lookAtPos = pos;
        updateView();
    }
}
