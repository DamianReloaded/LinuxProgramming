#include "bitmap.h"
#include <functional>

namespace owl
{
    // Class to hande window functionality
    class window
    {
        public:
            // define this function as a typedef so it's shorter to write.
            typedef std::function<void(window*)> event;

        public:
            window (owl::display* _display=NULL) : m_display(_display), m_window(0), 
                                                    m_running(false), m_backbuffer(_display)
            {
                
            }

            virtual ~window()
            {
                destroy();
            }

            void create(const int& _x, const int& _y, const int& _w=640, const int& _h=480, display* _display=NULL)
            {
                // If we are passing _display from here probably we didn't do it from the constructor
                if (_display != NULL)
                {
                    m_display = _display;
                    m_backbuffer.display(_display); // We use a bitmap as a backbuffer to draw
                }

                // Create a X window
                m_window = XCreateSimpleWindow(m_display->m_display, 
                                RootWindow(m_display->m_display, m_display->m_screen), 
                                _x, _y, _w, _h, 1, 
                                BlackPixel(m_display->m_display, m_display->m_screen), 
                                WhitePixel(m_display->m_display,m_display->m_screen));

                // Tell the X server which events we want
                XSelectInput(m_display->m_display, m_window, ExposureMask|KeyPressMask);
                // Show the window
                XMapWindow(m_display->m_display,m_window);
                // Create the backbuffer bitmap
                m_backbuffer.create(_w,_h);

                m_running = true;
            }

            void update()
            {
                // Get events
                XNextEvent(m_display->m_display, &m_event);
                if (m_event.type == Expose)
                {
                   // Call our drawing callback
                   on_expose(this);
                   // And draw the back buffer to the window
                   XPutImage(m_display->m_display,m_window,DefaultGC(m_display->m_display,0),
                                m_backbuffer.m_ximage,0,0,0,0,m_backbuffer.width(),
                                m_backbuffer.height());
                }
                else if (m_event.type==KeyPress)
                {
                    // If a key is pressed, we stop
                    m_running = false;
                }
                
                // force refresh every update for animations
                memset(&m_event, 0, sizeof(m_event));
                m_event.type = Expose;
                m_event.xexpose.window = m_window;
                XSendEvent(m_display->m_display, m_window, false, ExposureMask, &m_event);
                XFlush(m_display->m_display);

            }

            void blend(owl::bitmap* bmp, const int& _xd, const int& _yd, const int& _xo,
                           const int& _yo, const int& _w, const int& _h)
            {
                m_backbuffer.blend(bmp,_xd, _yd, _xo, _yo, _w, _h);
            }

            void destroy()
            {
                if (m_window!=0) XDestroyWindow(m_display->m_display, m_window); m_window=0;
            }

            const bool& running() { return m_running; }

            event on_expose;

        protected:
            bool            m_running;
            owl::display*   m_display;
            owl::bitmap     m_backbuffer;
            Window          m_window;
            XEvent          m_event;
    };
}
