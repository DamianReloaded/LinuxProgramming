#ifndef reload_sharedlib_h
#define reload_sharedlib_h

#include <string>

namespace reload
{
    class sharedlib
    {
        public:
                        sharedlib   ();
            virtual     ~sharedlib  ();

            const bool  open        (const std::string& _filepath);
            void        close       ();
            void*       get         (const std::string& _fname);
            std::string last_error  ();

        protected:
            class       implement;
            implement*  imp;
    };
}

#endif
