#include "taskbar.plugin.h"
#include "application.h"
#include <iostream>
using namespace reload;

taskbar::taskbar ()
{
}

taskbar::~taskbar()
{
}

bool taskbar::init(application* _app)
{
    std::cout << "Taskbar Plugin Initializing ... ";

    m_application = _app;
    m_skin.display(m_application->display());
    if (!m_skin.load("taskbar.skin.png"))
    {
        std::cout << "Can't load taskbar.skin.png" << std::endl;
    }

    m_window.create(0,_app->display()->screen_height()-m_skin.height(),
                    _app->display()->screen_width(), 
                    _app->display()->screen_height(), _app->display());

    m_window.on_keypress = std::bind(&taskbar::key_press, this, std::placeholders::_1);
    m_window.on_expose = std::bind(&taskbar::draw, this, std::placeholders::_1);

    m_application->taskbar = m_window.xwindow();

    std::cout << "Done." <<std::endl;
    return true;
}

void taskbar::draw(window* _window)
{
    _window->blend(&m_skin, 0,m_application->display()->screen_height()-m_skin.height(),0,0,
                        m_skin.width(), m_skin.height());
}

void taskbar::key_press(window*)
{
    m_application->end();
}

void taskbar::update()
{
    m_window.update();
}

void taskbar::terminate()
{
    std::cout << "Taskbar Plugin Terminating ...";
    m_window.destroy();
    m_skin.destroy();
    std::cout << "Done." << std::endl;
}

const std::string taskbar::last_error()
{
    return "";
}
