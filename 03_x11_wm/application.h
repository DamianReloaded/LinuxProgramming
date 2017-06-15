#ifndef reload_application_h
#define reload_application_h

#include <deque>
#include <string>
#include "plugin.h"
#include "display.h"

namespace reload {

    class application
    {
        public:
                                application     ();
            virtual             ~application    ();
            int                 run             ();
            void                update          ();
            void                end             ();
            void                list_files      (const std::string& _path, 
                                                    std::deque<std::string>& _list,
                                                    const std::string& _extension="");
            reload::display*    display         ();

            Window              desktop;
            Window              taskbar;

        protected:
            void                terminate       ();
            void                load_plugins    ();
            void                unload_plugins  ();

            bool                m_running;
            std::string         m_plugin_path;
            std::deque<plugin*> m_plugins;
            reload::display     m_display;
    };

}


#endif
