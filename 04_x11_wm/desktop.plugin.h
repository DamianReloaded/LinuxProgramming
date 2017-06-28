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
