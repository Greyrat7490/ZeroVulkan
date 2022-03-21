#include "ZSandboxCam.h"
#include "Input/Input.h"

namespace ZeroVulkan {
    void ZSandboxCam::update(float dt) {
        if (ZInput::isMouseMoving()) {
            if (ZInput::isHold(ZKey::LEFT_BUTTON)) {
                vec2 rot = ZInput::getMouseMotion() * rotSens;
                rot[0] = radian(rot[0]);
                rot[1] = radian(rot[1]);

                // TOOD: rotation doesn't feel natural if y-rotation is unequal 0°/180°
                // (because it's always just a z-rotation)

                rotate(rot * dt);
            } else if (ZInput::isHold(ZKey::RIGHT_BUTTON)) {
                vec2 motion = ZInput::getMouseMotion() * moveSens;

                vec3 right = normal(cross(vec3(0.f, 0.f, 1.f), m_direction));
                vec3 up = cross(m_direction, right);

                right *= motion[0];
                up *= motion[1];

                move((right - up) * dt);
            }
        } else {
            if (ZInput::isMouseWheelUp()) {
                move(m_direction * -zoomSens * dt);
            }

            if (ZInput::isMouseWheelDown()) {
                move(m_direction * zoomSens * dt);
            }
        }
    }
}
