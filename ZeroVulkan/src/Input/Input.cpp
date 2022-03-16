#include "Input.h"

namespace ZeroVulkan::ZInput
{
    static ZKeyState s_keys[UINT8_MAX];

    static vec2 s_mousePos;
    static vec2 s_motion;
    static bool s_mouseIsMoving;

    void setKeyState(uint8_t keyIdx, bool released) {
        if (released) {
            s_keys[keyIdx] = ZKeyState::RELEASED;
        } else {
            if (s_keys[keyIdx] == ZKeyState::TO_HOLD)
                s_keys[keyIdx] = ZKeyState::HOLD;
            else if (s_keys[keyIdx] != ZKeyState::HOLD)
                s_keys[keyIdx] = ZKeyState::PRESSED;
        }
    }

    void setCursorPos(int16_t x, int16_t y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;

        vec2 newPos = vec2(x, y);

        s_motion = s_mousePos - newPos;
        s_mousePos = newPos;
        s_mouseIsMoving = true;
    }

    void updateKeyMap() {
        s_mouseIsMoving = false;
        s_motion = vec2();

        for (uint8_t i = 0; i < UINT8_MAX; i++) {
            if (s_keys[i] == ZKeyState::RELEASED)
                s_keys[i] = ZKeyState::NONE;

            if (s_keys[i] == ZKeyState::PRESSED)
                s_keys[i] = ZKeyState::TO_HOLD;
        }
    }


    bool isPressed(uint8_t keyIdx) { return s_keys[keyIdx] == ZKeyState::PRESSED; }
    bool isHold(uint8_t keyIdx) { return (int)s_keys[keyIdx] > 1; }
    bool isReleased(uint8_t keyIdx) { return s_keys[keyIdx] == ZKeyState::RELEASED; }

    bool isMouseWheelUp() { return isPressed(4); }
    bool isMouseWheelDown() { return isPressed(5); }

    bool isMouseMoving() { return s_mouseIsMoving; }
    vec2 getMousePos() { return s_mousePos; }
    vec2 getMouseMotion() { return s_motion; }
}
