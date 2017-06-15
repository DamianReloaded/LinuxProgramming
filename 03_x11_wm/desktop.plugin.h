#ifndef reload_desktop_plugin_h
#define reload_desktop_plugin_h

#include "plugin.h"
#include "window.h"
#include "bitmap.h"

namespace reload {

    class application;

    class desktop : public plugin
    {
        public:
                                        desktop     ();
            virtual                     ~desktop    ();

            virtual bool                init        (application* _app);
            virtual void                update      ();
            virtual void                terminate   ();
            virtual const std::string   last_error  ();
        protected:
            void                        key_press   (window*);
            void                        draw        (window*);

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

#endif
