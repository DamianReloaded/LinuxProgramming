#include "application.h"
#include "sharedlib.h"
#include <iostream>
#include <dirent.h>
#include <unistd.h>

using namespace reload;

application::application()
{
    m_plugin_path = ".";
    desktop = 0;
    taskbar = 0;
}

application::~application()
{

}

void application::terminate()
{
    unload_plugins();
    m_display.close();
}

int application::run()
{
    m_running = true;

    if (!m_display.open())
    {
        std::cout << "Can't open display" << std::endl;
        return -1;
    }

    load_plugins();

    if (m_plugins.size()<1)
    {
        std::cout << "No plugins found in '" << m_plugin_path << "'." << std::endl;
        return -2;
    }

    while (m_running)
    {
        update();
    }

    return 0;
}

void application::update()
{
    for (size_t i=0; i<m_plugins.size(); i++)
    {
        plugin* plug = m_plugins[i];
        plug->update();
    }
    usleep(1000*1); // sleep 1 millisecond
}

void application::load_plugins()
{
    typedef plugin*(*plugin_get)();
    std::deque<std::string> files;
    list_files(m_plugin_path, files, "plugin.so");
    for (size_t i=0; i<files.size(); i++)
    {
        sharedlib* sl = new sharedlib();

        if (!sl->open(m_plugin_path + "/" + files[i]))
        {
            std::cout << "Can't open plugin '" << files[i] << "': " << sl->last_error() 
                        << std::endl;
            delete sl;
            continue;
        }

        plugin_get get = (plugin_get) sl->get("get");

        if (get==NULL)
        {
            std::cout << "Can't get an instance of plugin '" << files[i] << "': " 
                        << sl->last_error() << std::endl;
            sl->close();
            delete sl;
            continue;
        }

        plugin* p = get();
        if (!p->init(this))
        {
            std::cout << "Can't initialize plugin '" << files[i] << "': " << p->last_error()
                        << std::endl;
            sl->close();
            delete sl;
            continue;
        }

        p->sharedlib = sl;
        m_plugins.push_back(p);
    }
}

void application::unload_plugins()
{
    for (size_t i=0; i<m_plugins.size(); i++)
    {
        plugin* plug = m_plugins[i];
        plug->terminate();
        delete plug->sharedlib;
    }
}

void application::list_files(const std::string& _path, std::deque<std::string>& _list, 
                                const std::string& _extension)
{
    DIR* dr;
    struct dirent *drent;
    dr = opendir(_path.c_str());
    if (dr==NULL) return;

    bool allfiles = (_extension.length()<1);
    for (drent = readdir(dr); drent!=NULL; drent=readdir(dr))
    {
        std::string filename(drent->d_name);
        bool extension_match = false;
        if (filename.length()>=_extension.length())
        {
            std::string tmp = filename.substr(filename.length()-_extension.length(),
                                               _extension.length());
            extension_match = (tmp==_extension);
        }

        if (allfiles || extension_match)
        {
            _list.push_back(filename);
        }
    }

    closedir(dr);
}

reload::display* application::display()
{
    return &m_display;
}

void application::end()
{
    m_running = false;
}
