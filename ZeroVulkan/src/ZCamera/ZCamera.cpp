#include "ZCamera.h"
#include "Window/window.h"
#include <cstdint>

namespace ZeroVulkan
{
    ZCamera::ZCamera()
    {
        setAspect();
    }

    void ZCamera::setAspect(uint32_t width, uint32_t height)
    {
        m_size = { (float) width, (float) height };
        updateProj();
    }

    void ZCamera::setAspect()
    {
        m_size = ZWindow::getSize();
        updateProj();
    }


    void ZCamera::setPos(vec3 pos)
    {
        m_pos = pos;
        updateView();
    }
    void ZCamera::move(vec3 pos) {
        m_pos += pos;
        updateView();
    }

    void ZCamera::setRot(vec2 rot)
    {
        m_rot = rot;
        updateView();
    }

    void ZCamera::rotate(vec2 rot)
    {
        m_rot += rot;
        updateView();
    }

    void ZCamera::updateProj()
    {
        m_aspect = m_size[0] / m_size[1];

        m_proj[0][0] = 1.f / (m_aspect * std::tan(radian(m_fovY / 2.f)));
        m_proj[1][1] = -1.f / std::tan(radian(m_fovY / 2.f));// -1.f to flip y_axis
        m_proj[2][2] = m_farValue / (m_nearValue - m_farValue);
        m_proj[2][3] = -1.f;
        m_proj[3][2] = (m_farValue * m_nearValue) / (m_nearValue - m_farValue);
        m_proj[3][3] = 0.f;
    }
}
