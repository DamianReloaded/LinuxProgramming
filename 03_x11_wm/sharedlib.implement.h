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

#ifndef reload_sharedlib_implement_H
#define reload_sharedlib_implement_H

#include <dlfcn.h>
#include <string>

namespace reload {

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
			
			void* get(const std::string& _fname)
			{
				void* retval = dlsym(handle, _fname.c_str());
				if (!retval) last_error = dlerror();
				return retval;
			}
			
		std::string last_error;
		void* 		handle;
	};
}

#endif // reload_sharedlib_implement_H
