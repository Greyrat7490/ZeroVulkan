#include "Input.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace ZeroVulkan::ZInput
{
    enum class ZKeyState {
        NONE,
        RELEASED,
        PRESSED,
        TO_HOLD,
        HOLD
    };


    static KeyCode keycodes[UINT8_MAX];

    static ZKeyState s_keys[UINT8_MAX];

    static vec2 s_mousePos;
    static vec2 s_motion;
    static bool s_mouseIsMoving;

    void initKeymap() {
        Display* dpy = XOpenDisplay(0);

        uint8_t i = (int)ZKey::LEFT_BUTTON;

        keycodes[i++] = Button1;
        keycodes[i++] = Button2;
        keycodes[i++] = Button3;
        keycodes[i++] = Button4;
        keycodes[i]   = Button5;

        i = 8;

        keycodes[i++] = XKeysymToKeycode(dpy, XK_BackSpace);
        keycodes[i]   = XKeysymToKeycode(dpy, XK_Tab);

        i = 13;

        keycodes[i++] = XKeysymToKeycode(dpy, XK_Return);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Shift_L);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Shift_R);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Caps_Lock);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Control_L);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Control_R);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Alt_L);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Alt_R);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Super_L);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Super_R);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Menu);

        i = 27;

        keycodes[i]   = XKeysymToKeycode(dpy, XK_Escape);

        i = 32;

        keycodes[i]   = XKeysymToKeycode(dpy, XK_space);

        i = 42;

        keycodes[i++] = XKeysymToKeycode(dpy, XK_asterisk);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_plus);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_comma);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_minus);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_period);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_slash);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_0);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_1);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_2);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_3);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_4);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_5);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_6);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_7);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_8);
        keycodes[i]   = XKeysymToKeycode(dpy, XK_9);

        i = 65;

        keycodes[i++] = XKeysymToKeycode(dpy, XK_a);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_b);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_c);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_d);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_e);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_f);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_g);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_h);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_i);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_j);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_k);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_l);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_m);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_n);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_o);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_p);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_q);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_r);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_s);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_t);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_u);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_v);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_w);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_x);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_y);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_z);

        keycodes[i++] = XKeysymToKeycode(dpy, XK_udiaeresis);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_numbersign);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_odiaeresis);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_ssharp);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_dead_circumflex);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_dead_acute);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_adiaeresis);
        keycodes[i++] = 0; // TODO
        keycodes[i++] = XKeysymToKeycode(dpy, XK_less);

        i += 12;

        keycodes[i++] = XKeysymToKeycode(dpy, XK_F1);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F2);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F3);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F4);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F5);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F6);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F7);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F8);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F9);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F10);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F11);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F12);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F13);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F14);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F15);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F16);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F17);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F18);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F19);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F20);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F21);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F22);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F23);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_F24);

        keycodes[i++] = XKeysymToKeycode(dpy, XK_Num_Lock);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Scroll_Lock);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Pause);

        keycodes[i++] = XKeysymToKeycode(dpy, XK_Insert);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Delete);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Home);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_End);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Page_Up);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Page_Down);

        keycodes[i++] = XKeysymToKeycode(dpy, XK_Up);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Down);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Left);
        keycodes[i++] = XKeysymToKeycode(dpy, XK_Right);

        XCloseDisplay(dpy);
    }

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


    bool isPressed(ZKey key) { return s_keys[keycodes[(int)key]] == ZKeyState::PRESSED; }
    bool isHold(ZKey key) { return (int)s_keys[keycodes[(int)key]] > 1; }
    bool isReleased(ZKey key) { return s_keys[keycodes[(int)key]] == ZKeyState::RELEASED; }


    bool isMouseWheelUp() { return isPressed(ZKey::MOUSE_WHEEL_UP); }
    bool isMouseWheelDown() { return isPressed(ZKey::MOUSE_WHEEL_DOWN); }

    bool isMouseMoving() { return s_mouseIsMoving; }
    vec2 getMousePos() { return s_mousePos; }
    vec2 getMouseMotion() { return s_motion; }
}
