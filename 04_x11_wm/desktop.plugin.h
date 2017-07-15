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

#ifndef reload_desktop_plugin_H
#define reload_desktop_plugin_H

#include "plugin.h"
#include "window.h"
#include "bitmap.h"
#include <deque>
#include <thread>
#include <mutex>
namespace reload {

    class application;

    class desktop : public plugin
    {
        public:
                                        desktop         ();
            virtual                     ~desktop        ();

            virtual bool                init            (application* _app);
            virtual void                update          ();
            virtual void                terminate       ();
            virtual const std::string   last_error      ();

        protected:
            void                        keypress        (window*);
            virtual void                draw            (window*);

            application*                m_application;
            window                      m_window;
            bitmap                      m_wallpaper;
            bitmap                      m_characters;
            int                         m_screenw;
            int                         m_screenh;

            struct symbol
            {
                char  	value;
		double	last_change;
                float brightness;
            };
            typedef std::vector<symbol> array;
            std::vector<array>          m_matrix;

            struct stream
            {
                int   length;
        	float speed;
                std::vector<float> brightness;
                int   x;
                float y;
		double last_update;
            };
            std::vector<stream>         m_streams;
	    std::vector<uint8_t>	m_colused;
            std::thread                 m_thread;
            std::mutex                  m_mutex;
            static void                 thread_callback(desktop*);

        protected:
            void                        update_matrix();
            void                        draw_matrix();
            size_t                      m_last_update;
    };
}

extern "C"
{
    reload::plugin* get()
    {
        static reload::desktop instance;
        return &instance;
    }
}

#endif // reload_desktop_plugin_H
