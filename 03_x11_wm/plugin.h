#ifndef reload_plugin_h
#define reload_plugin_h

#include <string>

namespace reload {

    class application;
    class sharedlib;

    class plugin
    {
        public:
            virtual bool init (application* _app) = 0;
            virtual void update () = 0;
            virtual void terminate () = 0;
            virtual const std::string last_error() = 0;
            reload::sharedlib* sharedlib;
        protected:
            plugin () {}
    };

}


#endif
