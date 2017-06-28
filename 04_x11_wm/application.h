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

#ifndef reload_application_H
#define reload_application_H

#include <deque>
#include <string>
#include "plugin.h"
#include "display.h"

namespace reload {

    class application
    {
        public:
                                    application         ();
            virtual                 ~application        ();
            
            int                     run                 ();
            void                    update              ();
            void                    end                 ();
            void                    list_files          (const std::string& _path, std::deque<std::string>& _list, const std::string& _extension="");
            
            reload::display*        display             ();
            
            Window                  desktop;
            Window                  taskbar;
            
        protected:
            void                    terminate           ();
            void                    load_plugins        ();
            void                    unload_plugins      ();
            
            bool                    m_running;
            std::string             m_plugin_path;
            std::deque<plugin*>     m_plugins;
            reload::display         m_display;
    };

}

#endif // reload_application_H
