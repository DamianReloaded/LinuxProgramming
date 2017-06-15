#ifndef reload_taskbar_plugin_h
#define reload_taskbar_plugin_h

#include "plugin.h"
#include "window.h"
#include "bitmap.h"

namespace reload {

    class taskbar : public plugin
    {
        public:
                                            taskbar     ();
            virtual                         ~taskbar    ();

            virtual bool                    init        (application* _app);
            virtual void                    update      ();
            virtual void                    terminate   ();
            virtual const std::string       last_error  ();
        protected:
            void                            key_press   (window*);
            virtual void                    draw        (window*);

            application*                    m_application;
            window                          m_window;
            bitmap                          m_skin;
    };
}

extern "C"
{
    reload::plugin* get()
    {
        static reload::taskbar instance;
        return &instance;
    }
}

#endif
