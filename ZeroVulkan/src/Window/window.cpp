#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "ZRenderer/ZRenderer.h"

namespace ZeroVulkan::ZWindow
{
    xcb_window_t window;
    xcb_connection_t* connection;
    xcb_intern_atom_reply_t* wm_del_win;

    xcb_window_t getWindow() { return window; }
    xcb_connection_t* getConnection() { return connection; }
 
    uint16_t width;
    uint16_t height;

    void createWindow() {
        connection = xcb_connect(nullptr, nullptr);

        // get first screen
        const xcb_setup_t* setup = xcb_get_setup(connection);
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
        xcb_screen_t* screen = iter.data;

        uint32_t win_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        uint32_t win_values[2] = {
            screen->black_pixel,
            XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE
        };

        // create window
        window = xcb_generate_id(connection);
        xcb_create_window( 
            connection, 
            XCB_COPY_FROM_PARENT, 
            window, 
            screen->root, 
            0, 0, 
            800, 600, 
            0, 
            XCB_WINDOW_CLASS_INPUT_OUTPUT, 
            screen->root_visual, 
            win_mask, win_values );

        // setup close(delete) event handler
        xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom_unchecked(connection, 1, 12, "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply(connection, protocols_cookie, 0);
        xcb_intern_atom_cookie_t del_win_cookie = xcb_intern_atom_unchecked(connection, 0, 16, "WM_DELETE_WINDOW");
        wm_del_win = xcb_intern_atom_reply(connection, del_win_cookie, 0);
        xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, protocols_reply->atom, 4, 32, 1, &(wm_del_win->atom));
        free(protocols_reply);

        const char* title = "default title";
        xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);
     
        xcb_get_geometry_cookie_t geomCookie = xcb_get_geometry (connection, window);
        xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply (connection, geomCookie, nullptr);

        width = geom->width;
        height = geom->height;

        xcb_map_window(connection, window);
        xcb_flush(connection);
    }
    
    // return true if window is closed
    bool handleEvents() {
        xcb_generic_event_t* e;

        while ( ( e = xcb_poll_for_event(connection) ) ) {
            switch (e->response_type & ~0x80) {
                case XCB_CLIENT_MESSAGE:
                    if ( ( (xcb_client_message_event_t*)e )->data.data32[0] == wm_del_win->atom ) {
                        free(e);
                        return true;
                    }
                    break;
                case XCB_KEY_PRESS: {
                    xcb_key_press_event_t* kp = (xcb_key_press_event_t*)e;

                    printf("Key %d pressed\n", kp->detail);
                    break;
                }
                // resize event
                case XCB_CONFIGURE_NOTIFY: {
                    xcb_configure_notify_event_t* cfgEvent = (xcb_configure_notify_event_t*) e;
                    if (cfgEvent->width != width || cfgEvent->height != height) {
                        if (cfgEvent->width && cfgEvent->height) {
                            width = cfgEvent->width;
                            height = cfgEvent->height;

                            ZRenderer::resizing();
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            free(e);
        }

        return false;
    }
    
    void clear() {
        xcb_disconnect(connection);
    }

    vec2 getSize() {
        return vec2(width, height);
    }

    void setTitle(const std::string& title) {
        xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.size(), title.data());
        xcb_flush(connection);
    }
}
