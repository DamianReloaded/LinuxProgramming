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

#ifndef reload_plugin_H
#define reload_plugin_H

#include <string>

namespace reload {

	class application;
	class sharedlib;

	class plugin
	{
		public:
			virtual bool				init		(application* _app) = 0;
			virtual void				update		() = 0;
			virtual void				terminate	() = 0;
			virtual const std::string	last_error	() = 0;
			reload::sharedlib* 			sharedlib;
		protected:
			plugin	() {}
			
	};

}

#endif // reload_plugin_H
