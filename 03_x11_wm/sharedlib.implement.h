#ifndef reload_sharedlib_implement_h
#define reload_sharedlib_implement_h

#include <dlfcn.h>
#include <string>

namespace reload{

    class sharedlib::implement
    {
        public:
            implement() : handle(0) {}

            const bool open(const std::string& _path)
            {
                close();
                handle = dlopen(_path.c_str(), RTLD_LAZY);
                if (!handle) last_error = dlerror();
                return (handle!=NULL);
            }

            void close()
            {
                if (handle) dlclose(handle);
                handle = NULL;
            }

            void* get (const std::string& _fname)
            {
                void* retval = dlsym(handle, _fname.c_str());
                if (!retval) last_error = dlerror();
                return retval;
            }

            std::string last_error;
            void* handle;
    };

}

#endif
