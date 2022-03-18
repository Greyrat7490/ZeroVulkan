#ifndef INPUT_H_
#define INPUT_H_

#include "types.h"

namespace ZeroVulkan::ZInput
{
    enum class ZKeyState {
        NONE,
        RELEASED,
        PRESSED,
        TO_HOLD,
        HOLD
    };

    bool isPressed(uint8_t keyIdx);
    bool isHold(uint8_t keyIdx);
    bool isReleased(uint8_t keyIdx);

    bool isMouseWheelUp();
    bool isMouseWheelDown();

    bool isMouseMoving();
    vec2 getMousePos();
    vec2 getMouseMotion();
}

#endif // INPUT_H_
