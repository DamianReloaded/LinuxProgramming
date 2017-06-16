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

#include "taskbar.plugin.h"
#include "application.h"
#include <iostream>
using namespace reload;

taskbar::taskbar()
{

}

taskbar::~taskbar()
{

}

bool taskbar::init (application* _app)
{
    std::cout << "Taskbar Plugin: Init: ";
    
    m_application = _app;

    m_skin.display(m_application->display());
    if (!m_skin.load("taskbar.skin.png"))
        std::cout << "cant load bitmap" <<std::endl;
    
    m_window.create(0,_app->display()->screen_height()-m_skin.height(),_app->display()->screen_width(),_app->display()->screen_height(),_app->display());
    m_window.on_keypress = std::bind(&taskbar::keypress, this, std::placeholders::_1);
    m_window.on_expose = std::bind(&taskbar::draw, this, std::placeholders::_1);

    m_application->taskbar = m_window.xwindow();
    
    std::cout << "Done." <<std::endl;    
    
    return true;
}

void taskbar::draw (window* _window)
{
    _window->blend(&m_skin, 0, m_application->display()->screen_height()-m_skin.height(), 0, 0, m_skin.width(), m_skin.height());
}

void taskbar::keypress (window*)
{    
    //m_application->end();
}

void taskbar::update ()
{
    m_window.update();
}

void taskbar::terminate ()
{
    std::cout << "Taskbar Plugin: Terminate: ";
    m_window.destroy();
    m_skin.destroy();
    std::cout << "Done." <<std::endl;
}

const std::string taskbar::last_error ()
{
    return "";
}