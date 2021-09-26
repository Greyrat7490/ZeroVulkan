#ifndef WINDOW_H_
#define WINDOW_H_

#include <xcb/xcb.h>
#include <string>
#include "types.h"

namespace ZeroVulkan::ZWindow {
    void createWindow();
    void clear();
    bool handleEvents();
    vec2 getSize();
    void setTitle(const std::string& title);
    
    xcb_window_t getWindow();
    xcb_connection_t* getConnection();
}

#endif // WINDOW_H_
