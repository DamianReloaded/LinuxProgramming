#include <X11/Xlib.h>
#include <X11/Xutil.h>

class window;

namespace owl
{
    // Class to handle X11 display
    class display
    {
        public:
            const bool open()
            {
                m_display = XOpenDisplay(NULL);
                if (m_display==NULL) return false;
                m_screen = DefaultScreen(m_display);
                m_visual = DefaultVisual(m_display,m_screen);
                return true;
            }
    
            void close()
            {
                XCloseDisplay(m_display);
            }

        protected:
            Display*    m_display;
            Visual*     m_visual;
            int         m_screen;

        friend class window;
        friend class bitmap;
    };
}
