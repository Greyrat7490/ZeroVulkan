#include "ZFirstPersonCam.h"

namespace ZeroVulkan
{
    
    ZFirstPersonCam::ZFirstPersonCam() {
        updateView();
    }
    
    void ZFirstPersonCam::updateView() {
		vec3 right = {
            std::cos(m_rot[0]),
            -std::sin(m_rot[0]),
            0
        };
		vec3 up = { 
            std::sin(m_rot[0]) * std::sin(m_rot[1]), 
            std::cos(m_rot[0]) * std::sin(m_rot[1]),
            std::cos(m_rot[1]) 
        };
		m_direction = {
            -std::sin(m_rot[0]) * std::cos(m_rot[1]),
            -std::cos(m_rot[1]) * std::cos(m_rot[0]),
            std::sin(m_rot[1])
        };

		m_view[0] = vec4(right[0], up[0], m_direction[0], 0);
		m_view[1] = vec4(right[1], up[1], m_direction[1], 0);
		m_view[2] = vec4(right[2], up[2], m_direction[2], 0);
		m_view[3] = vec4(-dot(right, m_pos), -dot(up, m_pos), -dot(m_direction, m_pos), 1);
    }
}
