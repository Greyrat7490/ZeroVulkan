#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <xcb/xproto.h>

#include "ZRenderer/ZRenderer.h"


namespace ZeroVulkan::ZWindow
{
    static xcb_window_t s_window = 0;
    static xcb_connection_t* s_connection = nullptr;
    static xcb_intern_atom_reply_t* s_wm_del_win = nullptr;

    static uint16_t s_width = 0;
    static uint16_t s_height = 0;

    xcb_window_t getWindow() { return s_window; }
    xcb_connection_t* getConnection() { return s_connection; }

    void createWindow() {
        s_connection = xcb_connect(nullptr, nullptr);

        // get first screen
        const xcb_setup_t* setup = xcb_get_setup(s_connection);
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
        xcb_screen_t* screen = iter.data;

        uint32_t win_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        uint32_t win_values[2] = {
            screen->black_pixel,
            XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE
        };

        // create window
        s_window = xcb_generate_id(s_connection);
        xcb_create_window( 
            s_connection, 
            XCB_COPY_FROM_PARENT, 
            s_window, 
            screen->root, 
            0, 0, 
            800, 600, 
            0, 
            XCB_WINDOW_CLASS_INPUT_OUTPUT, 
            screen->root_visual, 
            win_mask, win_values );

        // setup close(delete) event handler
        xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom_unchecked(s_connection, 1, 12, "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply(s_connection, protocols_cookie, 0);
        xcb_intern_atom_cookie_t del_win_cookie = xcb_intern_atom_unchecked(s_connection, 0, 16, "WM_DELETE_WINDOW");
        s_wm_del_win = xcb_intern_atom_reply(s_connection, del_win_cookie, 0);
        xcb_change_property(s_connection, XCB_PROP_MODE_REPLACE, s_window, protocols_reply->atom, 4, 32, 1, &(s_wm_del_win->atom));
        free(protocols_reply);

        const char* title = "default title";
        xcb_change_property(s_connection, XCB_PROP_MODE_REPLACE, s_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);

        xcb_get_geometry_cookie_t geomCookie = xcb_get_geometry (s_connection, s_window);
        xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply (s_connection, geomCookie, nullptr);

        s_width = geom->width;
        s_height = geom->height;

        xcb_map_window(s_connection, s_window);
        xcb_flush(s_connection);
    }

    // return true if window is closed
    bool handleEvents() {
        xcb_generic_event_t* e;

        while ( ( e = xcb_poll_for_event(s_connection) ) ) {
            switch (e->response_type & ~0x80) {
                case XCB_CLIENT_MESSAGE:
                    if ( ( (xcb_client_message_event_t*)e )->data.data32[0] == s_wm_del_win->atom ) {
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
                    if (cfgEvent->width != s_width || cfgEvent->height != s_height) {
                        if (cfgEvent->width && cfgEvent->height) {
                            s_width = cfgEvent->width;
                            s_height = cfgEvent->height;

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
        xcb_disconnect(s_connection);
    }

    vec2 getSize() {
        return vec2(s_width, s_height);
    }

    void setTitle(const std::string& title) {
        if (!s_window || !s_connection) {
            puts("ERROR: no window created (you should create a ZProject object first)");
            return;
        }

        xcb_change_property(s_connection, XCB_PROP_MODE_REPLACE, s_window,
            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.size(), title.data());
        xcb_flush(s_connection);
    }
}
