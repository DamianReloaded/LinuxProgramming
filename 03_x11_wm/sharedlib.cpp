#include "sharedlib.h"
#include "sharedlib.implement.h"

using namespace reload;

sharedlib::sharedlib()
{
    imp = new implement();
}

sharedlib::~sharedlib()
{
    delete imp;
}

const bool sharedlib::open (const std::string& _filepath)
{
    return imp->open(_filepath);
}

void sharedlib::close()
{
    imp->close();
}

void* sharedlib::get (const std::string& _fname)
{
    return imp->get(_fname);
}

std::string sharedlib::last_error()
{
    return imp->last_error;
}
