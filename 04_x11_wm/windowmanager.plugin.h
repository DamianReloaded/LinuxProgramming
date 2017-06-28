/*
ReLoad Window Manager

Copyright (c) 2013-2014 Damian Reloaded

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef reload_windowmanager_plugin_H
#define reload_windowmanager_plugin_H

#include "plugin.h"
#include <unordered_map>
#include <mutex>
#include "window.h"

namespace reload {

    typedef std::unordered_map<Window, Window> winmap;    
    
    template <class T, int size>
    struct point
    {
        T       v[size];
                point        () {for(size_t i=0; i<size; i++)v[i]=0;}
                point        (const int32_t _v1, const int32_t _v2) { v[0]=_v1; v[1]=_v2; }
        point   operator -    (const point& _p) { return point(v[0]-_p.v[0], v[1]-_p.v[1]); }
        point   operator +    (const point& _p) { return point(v[0]+_p.v[0], v[1]+_p.v[1]); }
    };

    typedef point<int32_t,2> point2i;
    
    class windowmanager : public plugin
    {
        public:
                                windowmanager           ();
            virtual             ~windowmanager          ();

            bool                init                    (application* _app);
            void                update                  ();
            void                terminate               ();    
            const std::string    last_error             ();

            void                decorate                (const Window& w, const bool& _existed);
            void                undecorate              (const Window& w);
            void                on_unmap                (const XUnmapEvent& e);
            void                on_map_request          (const XMapRequestEvent& e);
            void                on_configure_request    (const XConfigureRequestEvent& e);
            void                on_key_press            (const XKeyEvent& e);
            void                on_button_press         (const XButtonEvent& e);
            void                on_button_release       (const XButtonEvent& e);
            void                on_motion_notify        (const XMotionEvent& e);

            void                wm_delete               (Display* _display, Window _window);            
            
            display*            m_display;
            winmap              nonclients;
            winmap              clients;

            bool                moving;
            bool                resizing;

            point2i             drag_start_location;
            point2i             drag_window_location;
            point2i             drag_window_size;

            uint32_t            titlebar_size;
            uint32_t            border_size;
            uint32_t            border_color;
            uint32_t            background_color;
            application*        m_application;
    };

    extern "C" 
    {
        reload::plugin* get()
        {
            static reload::windowmanager instance;
            return &instance;
        }
    }
}

#endif // reload_windowmanager_plugin_H
