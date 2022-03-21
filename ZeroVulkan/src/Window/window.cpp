#include "window.h"

#include <X11/X.h>
#include <X11/Xlib.h>

#include "ZRenderer/ZRenderer.h"
#include "Input/Input.h"


namespace ZeroVulkan::ZInput {
    void setKeyState(uint8_t keyIdx, bool released);
    void setCursorPos(int16_t x, int16_t y);
    void updateKeyMap();
}

namespace ZeroVulkan::ZWindow
{
    static Window s_window;
    static Display* s_display;

    static Atom s_wm_del_win;

    static uint16_t s_width;
    static uint16_t s_height;

    Window getWindow() { return s_window; }
    Display* getDisplay() { return s_display; }

    void createWindow() {
        ZInput::initKeymap();

        s_display = XOpenDisplay(nullptr);
        if (!s_display) {
            puts("ERROR: cound not open X11 display");
            exit(1);
        }

        int screen = XDefaultScreen(s_display);

        s_window = XCreateSimpleWindow(
                s_display,
                XRootWindow(s_display, screen),
                0, 0,
                800, 600, 1,
                BlackPixel(s_display, screen), WhitePixel(s_display, screen));

        XSelectInput(s_display, s_window, StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                PointerMotionMask | ButtonPressMask | ButtonReleaseMask);

        XStoreName(s_display, s_window, "default title");

        s_wm_del_win = XInternAtom(s_display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(s_display, s_window, &s_wm_del_win, 1);

        XWindowAttributes wa;
        XGetWindowAttributes(s_display, s_window, &wa);
        s_width = wa.width;
        s_height = wa.height;

        XMapWindow(s_display, s_window);
        XFlush(s_display);
    }

    // return true if window is closed
    bool handleEvents() {
        ZInput::updateKeyMap();

        while (XPending(s_display)) {
            XEvent e;
            XNextEvent(s_display, &e);

            switch (e.type) {
                case KeyRelease: {
                    // TODO: disable auto repeat
                    // if auto repeat is enabled in X11 key holding gets converted to key press and release events
                    if (XPending(s_display)) {
                        XEvent ne;
                        XNextEvent(s_display, &ne);
                        if (ne.type == KeyPress && ne.xkey.time == e.xkey.time && ne.xkey.keycode == e.xkey.keycode) {
                            // ignore key release event and just handle the key press event
                            ZInput::setKeyState(ne.xkey.keycode, false);
                            continue;
                        }
                    }

                    ZInput::setKeyState(e.xkey.keycode, true);
                    break;
                }
                case ButtonRelease:
                    ZInput::setKeyState(e.xbutton.button, true);
                    break;
                case ButtonPress: {
                        ZInput::setKeyState(e.xbutton.button, false);

                        if (e.xbutton.button == 4 || e.xbutton.button == 5) // mouse wheel up or down
                            return false;

                        break;
                    }
                case KeyPress:
                    ZInput::setKeyState(e.xkey.keycode, false);
                    break;
                case MotionNotify: {
                    ZInput::setCursorPos(e.xmotion.x, e.xmotion.y);
                    break;
                }
                // window closed
                case ClientMessage:
                    if (static_cast<Atom>(e.xclient.data.l[0]) == s_wm_del_win) {
                        return true;
                    }
                    break;
                // resize event
                case ConfigureNotify: {
                    if (e.xconfigure.width != s_width || e.xconfigure.height != s_height) {
                        if (e.xconfigure.width && e.xconfigure.height) {
                            s_width = e.xconfigure.width;
                            s_height = e.xconfigure.height;

                            ZRenderer::resizing();
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        return false;
    }

    void clear() {
        XDestroyWindow(s_display, s_window);
#ifdef Z_DEBUG
        // vkDestroySwapchainKHR later causes a segmentation fault
        // if validation layer is disabled (seemse to be another NVidia Driver bug...)
        // https://gitlab.freedesktop.org/xorg/lib/libxext/-/issues/3
        // https://github.com/KhronosGroup/Vulkan-Docs/issues/1634
        XCloseDisplay(s_display);
#endif
    }

    vec2 getSize() {
        return vec2(s_width, s_height);
    }

    void setTitle(const std::string& title) {
        if (!s_window || !s_display) {
            puts("ERROR: no window created (you should create a ZProject object first)");
            return;
        }

        XStoreName(s_display, s_window, title.c_str());
        XFlush(s_display);
    }
}
