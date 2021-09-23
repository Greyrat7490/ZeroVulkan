#ifndef WINDOW_H_
#define WINDOW_H_

#include <xcb/xcb.h>

namespace ZeroVulkan::ZWindow {
    void createWindow();
    void clear();
    bool handleEvents();

    xcb_window_t getWindow();
    xcb_connection_t* getConnection();
}

#endif // WINDOW_H_
