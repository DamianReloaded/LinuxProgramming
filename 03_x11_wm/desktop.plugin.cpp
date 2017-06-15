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

bool desktop::init(application* _app)
{
    std::cout << "Desktop Plugin: Init ... ";

    m_application = _app;
    m_window.create(0,0,_app->display()->screen_width(),_app->display()->screen_height(),
                        _app->display());
    m_window.on_keypress = std::bind(&desktop::key_press, this, std::placeholders::_1);
    m_window.on_expose = std::bind(&desktop::draw, this, std::placeholders::_1);

    m_application->desktop = m_window.xwindow();

    m_wallpaper.display(m_application->display());
    if (!m_wallpaper.load("wallpaper.png"))
    {
        std::cout << "Can't load bitmap wallpaper.png" << std::endl;
    }
    else
    {
        m_window.blend(&m_wallpaper,0,0,0,0,m_wallpaper.width(),m_wallpaper.height());
    }

    std::cout << "Done." << std::endl;
    return true;
}

void desktop::draw(window* _window)
{
}

void desktop::key_press(window* _window)
{
}

void desktop::update()
{
    m_window.update();
}

void desktop::terminate()
{
    std::cout << "Desktop Plugin: Terminating ... ";
    m_window.destroy();
    m_wallpaper.destroy();
    std:: cout << "Done." <<std::endl;
}

const std::string desktop::last_error()
{
    return "";
}
