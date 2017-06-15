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

#include "bitmap.h"
#include <functional>

namespace reload
{
    // Class to hande window functionality
    class window
    {
        public:
            // define this function as a typedef so it's shorter to write.
            typedef std::function<void(window*)> event;

        public:
            window (reload::display* _display=NULL) : m_display(_display), m_window(0), 
														m_backbuffer(_display)
            {
                m_event_mask = ExposureMask|KeyPressMask;
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
                                RootWindow(m_display->m_display, m_display->m_screen_number), 
                                _x, _y, _w, _h, 0, 
                                WhitePixel(m_display->m_display,m_display->m_screen_number), 
                                BlackPixel(m_display->m_display, m_display->m_screen_number));

                // Tell the X server which events we want
                XSelectInput(m_display->m_display, m_window, m_event_mask);
                // Show the window
                XMapWindow(m_display->m_display,m_window);
                // Create the backbuffer bitmap
                m_backbuffer.create(_w,_h);
            }

            void update()
            {
				/*
				XPeekEvent(m_display->m_display, &m_event);
				if (m_event.xany.window != m_window) return;
				
                // Get events
                XNextEvent(m_display->m_display, &m_event);
				*/
				
				if(!XCheckWindowEvent(m_display->m_display, m_window, m_event_mask, &m_event)) return;
				
                switch (m_event.type)
				{
					case Expose:
					{
					   // Call our drawing callback
					   if(on_expose) on_expose(this);
					   // And draw the back buffer to the window
					   XPutImage(m_display->m_display,m_window,DefaultGC(m_display->m_display,0),
									m_backbuffer.m_ximage,0,0,0,0,m_backbuffer.width(),
									m_backbuffer.height());
					} break;
					
					case KeyPress:
					{
						if(on_keypress) on_keypress(this);
						
					} break;

					case ButtonPress:
					{
						switch (m_event.xbutton.button)
						{
						case 1:
						break;
						case 2:
						break;
						case 3:
						break;
						}

						break;
					}

					case ConfigureNotify:
					{
						/*
						w->m_control->m_width = event.xconfigure.width;
						w->m_control->m_height = event.xconfigure.height;
						w->m_backbuffer.resize(w->m_control->m_width, w->m_control->m_height);
						*/
						//event.xconfigure.width, event.xconfigure.height
						/* fall through... */
					} break;
					
				}//switch
				
				// force refresh every update for animations
				/*
				memset(&m_event, 0, sizeof(m_event));
				m_event.type = Expose;
				m_event.xexpose.window = m_window;
				XSendEvent(m_display->m_display, m_window, false, ExposureMask, &m_event);
				XFlush(m_display->m_display);
				*/
			}
			
            void blend(reload::bitmap* bmp, const int& _xd, const int& _yd, const int& _xo,
                           const int& _yo, const int& _w, const int& _h)
            {
                m_backbuffer.blend(bmp,_xd, _yd, _xo, _yo, _w, _h);
            }

            void destroy()
            {
                if (m_window!=0) XDestroyWindow(m_display->m_display, m_window); 
				m_window=0;
            }

			Window& xwindow() { return m_window; }
			
            event on_expose;
			event on_keypress;

        protected:
            reload::display*	m_display;
            reload::bitmap		m_backbuffer;
            Window				m_window;
            XEvent          	m_event;
			long				m_event_mask;
    };
}
