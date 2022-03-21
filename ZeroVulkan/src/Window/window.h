#ifndef WINDOW_H_
#define WINDOW_H_

#include <X11/Xlib.h>
#include <string>
#include "types.h"

namespace ZeroVulkan::ZWindow {
    void createWindow();
    void clear();
    bool handleEvents();
    vec2 getSize();
    void setTitle(const std::string& title);

    Window getWindow();
    Display* getDisplay();
}

#endif // WINDOW_H_
