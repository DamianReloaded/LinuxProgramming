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
   in a product, an acknreloadedgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef reload_display_H
#define reload_display_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>

class window;

namespace reload
{
    // Class to handle X11 display
    class display
    {
        public:
            const bool open()
            {
                m_display = XOpenDisplay(NULL);
                if (m_display==NULL) return false;
                m_screen_number = DefaultScreen(m_display);
                m_screen = XDefaultScreenOfDisplay(m_display);
                m_visual = DefaultVisual(m_display,m_screen_number);
                m_root = DefaultRootWindow(m_display);
                WM_PROTOCOLS = XInternAtom(m_display, "WM_PROTOCOLS", false);
                WM_DELETE_WINDOW = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
                return true;
            }
    
            void close()
            {
                XCloseDisplay(m_display);
            }
        
            int screen_width                () {return WidthOfScreen(m_screen); }
            int screen_height               () {return HeightOfScreen(m_screen); }
        
            Display*        xdisplay        () { return m_display; };
            Visual*         xvisual         () { return m_visual; };
            int             xscreen_number  () { return m_screen_number; };
            Screen*         xscreen         () { return m_screen; };
            Window&         root            () { return m_root; }
            
            static Atom     WM_PROTOCOLS;
            static Atom     WM_DELETE_WINDOW;
        
        protected:
            Display*        m_display;
            Visual*         m_visual;
            int             m_screen_number;
            Screen*         m_screen;
            Window          m_root;

        friend class window;
        friend class bitmap;
    };

}

#endif // reload_display_H