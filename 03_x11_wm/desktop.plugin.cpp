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

#include "desktop.plugin.h"
#include "application.h"
#include <iostream>
using namespace reload;

desktop::desktop()
{

}

desktop::~desktop()
{

}

bool desktop::init (application* _app)
{
	std::cout << "Desktop Plugin: Init: ";
	
	m_application = _app;
	
	m_window.create(0,0,_app->display()->screen_width(),_app->display()->screen_height(),_app->display());
	m_window.on_keypress = std::bind(&desktop::keypress, this, std::placeholders::_1);
	m_window.on_expose = std::bind(&desktop::draw, this, std::placeholders::_1);

	m_application->desktop = m_window.xwindow();
	
	m_wallpaper.display(m_application->display());
	if (!m_wallpaper.load("wallpaper.png"))
		std::cout << "cant load bitmap" <<std::endl;

	m_window.blend(&m_wallpaper, 0, 0, 0, 0, m_wallpaper.width(), m_wallpaper.height());
	
	std::cout << "Done." <<std::endl;
	return true;
}

void desktop::draw (window* _window)
{
	
}

void desktop::keypress (window*)
{	
	//m_application->end();
}

void desktop::update ()
{
	m_window.update();
}

void desktop::terminate ()
{
	std::cout << "Desktop Plugin: Terminate: ";
	m_window.destroy();
	m_wallpaper.destroy();
	std::cout << "Done." <<std::endl;
}

const std::string desktop::last_error ()
{
	return "";
}