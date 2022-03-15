#include "Input.h"

namespace ZeroVulkan::ZInput
{
    static ZKeyState s_keys[UINT8_MAX];

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

    bool isPressed(uint8_t keyIdx) {
        return s_keys[keyIdx] == ZKeyState::PRESSED;
    }

    bool isHold(uint8_t keyIdx) {
        return (int)s_keys[keyIdx] > 1;
    }
    
    bool isReleased(uint8_t keyIdx) {
        return s_keys[keyIdx] == ZKeyState::RELEASED;
    }

    void updateKeyMap() {
        for (uint8_t i = 0; i < UINT8_MAX; i++) {
            if (s_keys[i] == ZKeyState::RELEASED)
                s_keys[i] = ZKeyState::NONE;

            if (s_keys[i] == ZKeyState::PRESSED)
                s_keys[i] = ZKeyState::TO_HOLD;
        }
    }
}
