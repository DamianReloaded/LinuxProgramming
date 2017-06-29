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
#include <unistd.h>
#include <sys/time.h>
using namespace reload;

double get_ticks()
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec*1000LL + spec.tv_nsec/1000000;

}

void desktop::thread_callback(desktop* dsk)
{
    while(true)
    {
        dsk->update_matrix();
        dsk->m_mutex.lock();
        dsk->draw_matrix();
        dsk->m_window.force_refresh();
        dsk->m_mutex.unlock();
        usleep(1000*1);
    }
}

desktop::desktop()
{
    m_last_update = get_ticks();
}

desktop::~desktop()
{

}

bool desktop::init (application* _app)
{
	std::cout << "Desktop Plugin: Init: ";

    m_application = _app;

    m_screenw = _app->display()->screen_width();
    m_screenh = _app->display()->screen_height();

	m_window.create(0,0,m_screenw,m_screenh,_app->display());
	m_window.on_keypress = std::bind(&desktop::keypress, this, std::placeholders::_1);
	m_window.on_expose = std::bind(&desktop::draw, this, std::placeholders::_1);

	m_application->desktop = m_window.xwindow();

	m_wallpaper.display(m_application->display());
	m_wallpaper.create(m_screenw,m_screenh);
    m_wallpaper.clear(color(0,0,0,255));

    m_characters.display(m_application->display());
    if (!m_characters.load("mrainch20.png"))
        std::cout << "can't load characters bitmap" <<std::endl;

	m_window.blend(&m_wallpaper, 0, 0, 0, 0, m_screenw, m_screenh);

    srand(time(NULL));

    m_matrix.resize(m_screenw/20);
    for (size_t c=0; c<m_matrix.size(); c++)
    {
         m_matrix[c].resize(m_screenh/20);
        for (size_t r=0; r<m_matrix[c].size(); r++)
        {
            m_matrix[c][r].value=rand()%54;
            m_matrix[c][r].brightness = 0;
        }
    }

    m_streams.resize((m_screenw/20)*3.14f);
    for (size_t i=0; i<m_streams.size(); i++)
    {
        m_streams[i].x = rand()%(m_screenw/20);
        m_streams[i].y = rand()%(m_screenh/20*2)+(m_screenh/20);
	m_streams[i].speed = rand()%4+1;
        m_streams[i].length = rand()%int((m_screenh/20)*0.75f)+3;
        m_streams[i].brightness.resize(m_streams[i].length);
        size_t bsize = m_streams[i].brightness.size();
        for (size_t b=1; b<bsize-1;b++)
        {
            m_streams[i].brightness[(bsize-1)-b] = 0.7f * ( float(b)/float(bsize) );
        }
        m_streams[i].brightness[2]=0.8f;
        m_streams[i].brightness[1]=0.9f;
        m_streams[i].brightness[0]=1.0f;
    }

    m_thread = std::thread(desktop::thread_callback,this);
    m_thread.detach();
	std::cout << "Done." <<std::endl;
	return true;
}

void desktop::update_matrix()
{
    m_wallpaper.clear(color(0,0,0,255));

    for (size_t i=0; i<m_streams.size(); i++)
    {
        for (size_t n=0; n<m_streams[i].length; n++)
        {
            int x = m_streams[i].x;
            int y = m_streams[i].y+n;
            if (y>-1 && y<m_matrix[0].size())
            {
                m_matrix[x][y].brightness=m_streams[i].brightness[n];
            }
        }
        m_streams[i].y-=m_streams[i].speed;

        if ((m_streams[i].y+m_streams[i].length)<0)
        {
        m_streams[i].x = rand()%(m_screenw/20);
        m_streams[i].y = rand()%(m_screenh/20*2)+(m_screenh/20);
	m_streams[i].speed = rand()%4+1;
        m_streams[i].length = rand()%int((m_screenh/20)*0.75f)+3;
        m_streams[i].brightness.resize(m_streams[i].length);
        size_t bsize = m_streams[i].brightness.size();
        for (size_t b=1; b<bsize-1;b++)
        {
            m_streams[i].brightness[(bsize-1)-b] = 0.7f * ( float(b)/float(bsize) );
        }
        m_streams[i].brightness[2]=0.8f;
        m_streams[i].brightness[1]=0.9f;
        m_streams[i].brightness[0]=1.0f;
        }
    }

    for (size_t c=0; c<m_matrix.size(); c++)
    {
        for (size_t r=0; r<m_matrix[c].size(); r++)
        {
            if (rand()%11==5) m_matrix[c][r].value = rand()%54;
            if (m_matrix[c][r].brightness>=0.05)
            {
                m_wallpaper.blend(&m_characters,c*20,r*20,0,m_matrix[c][r].value*20,20,20,m_matrix[c][r].brightness);
		m_matrix[c][r].brightness-=0.1;
            }
	    else
	    {
		m_matrix[c][r].brightness=0;
	    }
        }
    }
}

void desktop::draw_matrix()
{
//    m_mutex.lock();
    m_window.blend(&m_wallpaper,0,0,0,0,1920,1080);
//    m_mutex.unlock();
}

void desktop::draw (window* _window)
{
    //m_window.force_refresh();
}

void desktop::update ()
{
    m_mutex.lock();
	m_window.update();
    m_mutex.unlock();
/*
    double now = get_ticks();
    if (now-m_last_update>200)
    {
        update_matrix();
        draw_matrix();
        m_last_update = now;
        m_window.force_refresh();
    }
*/
}

void desktop::keypress(window*) {}

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
