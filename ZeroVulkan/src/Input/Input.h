#ifndef INPUT_H_
#define INPUT_H_

#include "types.h"

namespace ZeroVulkan::ZInput
{
    void initKeymap();

    bool isPressed(ZKey key);
    bool isHold(ZKey key);
    bool isReleased(ZKey key);

    bool isMouseWheelUp();
    bool isMouseWheelDown();

    bool isMouseMoving();
    vec2 getMousePos();
    vec2 getMouseMotion();
}

#endif // INPUT_H_
