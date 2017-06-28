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

#include "windowmanager.plugin.h"
#include "application.h"
#include "display.h"

using namespace reload;

struct window_geometry
{
    Window      root;
    int         x, y;
    unsigned    width, height, bsize, depth;
};

struct querytree
{
    Window      root, parent;
    Window*     windows;
    uint32_t    count;
                querytree(Display* _display, Window& root) { XQueryTree(_display, root, &root, &parent, &windows, &count); }
};

struct errorcheck
{
                                errorcheck  () : failed(false), lock(mutex) {self=this; XSetErrorHandler(&errorcheck::callback);}
    virtual                     ~errorcheck () {self = NULL;}
    static int                  callback    (Display* display, XErrorEvent* e) { if (self!=NULL) self->failed = true; return 0; }
    bool                        failed;
    static errorcheck*          self;
    std::mutex                  mutex;
    std::lock_guard<std::mutex> lock;
};
errorcheck* errorcheck::self=NULL;    

int xerrorhandler(Display* display, XErrorEvent* e)
{
    std::string error;
    error += "XErrorHandler Request Code: " + e->request_code;
    error += ". Error Code: " + e->error_code;
    std::cout << "WindowManager Error:"  << error;
    return 0;
}

windowmanager::windowmanager()
{
    titlebar_size         = 25;
    border_size         = 4;
    background_color    = 0x0000ff;
    border_color        = background_color;
}

windowmanager::~windowmanager()
{

}

bool windowmanager::init (application* _app)
{
    std::cout << "WindowManager Plugin: Init: ";

    m_application = _app;
    m_display = _app->display();
    
    { // register wm
        errorcheck check;
        XSelectInput(m_display->xdisplay(), m_display->root(), SubstructureRedirectMask | SubstructureNotifyMask);
        XSync(m_display->xdisplay(), false);
        if (check.failed) return false;
    }
    
    XSetErrorHandler(&xerrorhandler);
    XGrabServer(m_display->xdisplay());
    querytree wq(m_display->xdisplay(), m_display->root());
    for (uint32_t i=0; i<wq.count; i++) decorate(wq.windows[i], true);
    XFree(wq.windows);
    XUngrabServer(m_display->xdisplay());

    std::cout << "Done." <<std::endl;    
    return true;
}

void windowmanager::update ()
{
    #define skipmotion() while (XCheckTypedWindowEvent(m_display->xdisplay(), e.xmotion.window, MotionNotify, &e))
        
    XEvent e;
    XNextEvent(m_display->xdisplay(), &e);
    switch (e.type)
    {
        case UnmapNotify:       on_unmap(e.xunmap);                              break;
        case MapRequest:        on_map_request(e.xmaprequest);                   break;
        case ConfigureRequest:  on_configure_request(e.xconfigurerequest);       break;
        case KeyPress:          on_key_press(e.xkey);                            break;
        case ButtonPress:       on_button_press(e.xbutton);                       break;
        case ButtonRelease:     on_button_release(e.xbutton);                    break;
        case MotionNotify:      skipmotion(); on_motion_notify(e.xmotion);       break;
        case ResizeRequest: break;
        //case EnterNotify:         XRaiseWindow(display, e.xcrossing.window); break;
        //case FocusIn: XRaiseWindow(display, e.xfocus.window); break;
        default: break;
    }    
}

void windowmanager::on_unmap(const XUnmapEvent& e)
{
    if (nonclients.count(e.window)<1) return;
    if (e.event == m_display->root()) return;
    undecorate(e.window);
}

void windowmanager::on_map_request(const XMapRequestEvent& e)
{
    decorate(e.window, false);
    XMapWindow(m_display->xdisplay(), e.window);
}

void windowmanager::on_configure_request(const XConfigureRequestEvent& e)
{
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;
    if (nonclients.count(e.window))
    {
        const Window nonclient = nonclients[e.window];
        XConfigureWindow(m_display->xdisplay(), nonclient, e.value_mask, &changes);
    }
    XConfigureWindow(m_display->xdisplay(), e.window, e.value_mask, &changes);
}

void windowmanager::on_button_press(const XButtonEvent& e)
{
    // Click on a grabbed client
    if (nonclients.count(e.window))
    {
        XAllowEvents(m_display->xdisplay(), ReplayPointer, CurrentTime);
        XRaiseWindow(m_display->xdisplay(), nonclients[e.window]);
    }

    // We only handle clicks for nonclient windows
    if (clients.count(e.window)>0)
    {
        const Window nonclient = e.window;
        drag_start_location = point2i(e.x_root, e.y_root);
        window_geometry wg;
        XGetGeometry(m_display->xdisplay(), nonclient, &wg.root, &wg.x, &wg.y, &wg.width, &wg.height, &wg.bsize, &wg.depth);
        drag_window_location = point2i(wg.x, wg.y);
        drag_window_size = point2i(wg.width, wg.height);
        moving = (e.y<int(titlebar_size));
        resizing = e.x_root>int(wg.x+wg.width+wg.bsize) && e.y_root>int(wg.y+wg.height+wg.bsize);
        XRaiseWindow(m_display->xdisplay(), nonclient);
    }
}

void windowmanager::on_button_release(const XButtonEvent& e)
{
    moving    = false;
    resizing  = false;
}

void windowmanager::on_motion_notify(const XMotionEvent& e)
{
    if (clients.count(e.window)<1) return;
        
    point2i drag_pos(e.x_root, e.y_root);
    point2i delta = drag_pos - drag_start_location;

    if (e.state & Button1Mask )
    {
        if (moving)
        {
            const point2i dest_nonclient_pos = drag_window_location + delta;
            XMoveWindow(m_display->xdisplay(), e.window, dest_nonclient_pos.v[0], dest_nonclient_pos.v[1]);
        }
        else if (resizing)
        {
            const point2i size_delta( std::max(delta.v[0], -drag_window_size.v[0]), std::max(delta.v[1], -drag_window_size.v[1]));
            const point2i dest_nonclient_size = drag_window_size + size_delta;
            XResizeWindow(m_display->xdisplay(), e.window, dest_nonclient_size.v[0], dest_nonclient_size.v[1]);
            XResizeWindow(m_display->xdisplay(), clients[e.window], dest_nonclient_size.v[0], dest_nonclient_size.v[1]-titlebar_size);
        }
    }
}

void windowmanager::wm_delete(Display* _display, Window _window)
{
    // Send WM_DELETE_WINDOW message
    Atom* protocols;
    int count;
    if ( XGetWMProtocols(_display, _window, &protocols, &count) )
    {
        for(Atom* p = protocols; p!=protocols+count; p++)
        {
            if (m_display->WM_DELETE_WINDOW==*p)
            {
                XEvent msg;
                memset(&msg, 0, sizeof(msg));
                msg.xclient.type = ClientMessage;
                msg.xclient.message_type = m_display->WM_PROTOCOLS;
                msg.xclient.window = _window;
                msg.xclient.format = 32;
                msg.xclient.data.l[0] = m_display->WM_DELETE_WINDOW;
                XSendEvent(_display, _window, false, 0, &msg);
                return;
            }
        }
    }
    
    // WM_DELETE_WINDOW not supported. Just kill it with fire.
    XKillClient(_display, _window);
}

void windowmanager::on_key_press(const XKeyEvent& e)
{
    // Handle Alt-F4
    if ((e.state & Mod1Mask) && (e.keycode == XKeysymToKeycode(m_display->xdisplay(), XK_F4)))
    {
        wm_delete(m_display->xdisplay(),e.window);
    }
    // Handle Alt-Tab
    else if ((e.state & Mod1Mask) && (e.keycode == XKeysymToKeycode(m_display->xdisplay(), XK_Tab)))
    {
        auto i = nonclients.find(e.window); 
        if(i != nonclients.end()) i++;
        if(i == nonclients.end()) i = nonclients.begin();
        if(i == nonclients.end()) return;
        XRaiseWindow(m_display->xdisplay(), i->second);
        XSetInputFocus(m_display->xdisplay(), i->first, RevertToPointerRoot, CurrentTime);
    }
}

void windowmanager::terminate ()
{
    
}

void windowmanager::decorate (const Window& w, const bool& _existed)
{
    // Don't decorate windows already processed
    if (nonclients.count(w)>0) return;
    // Exclude the desktop and the taskbar
    if (w==m_application->desktop) return;
    if (w==m_application->taskbar) return;
    
    XWindowAttributes attrs;
    XGetWindowAttributes(m_display->xdisplay(), w, &attrs);

    // don't process these
    if (_existed && (attrs.override_redirect || attrs.map_state!=IsViewable) ) return;

    int new_x = attrs.x-border_size; if(new_x<0) new_x = 0;
    int new_y = attrs.y-(titlebar_size+border_size); if(new_y<0) new_y = 0;
    int new_width = attrs.width;
    int new_height = attrs.height+(titlebar_size);

    // Create nonclient window
    const Window nonclient = XCreateSimpleWindow(m_display->xdisplay(), m_display->root(), new_x, new_y, new_width, new_height, border_size, border_color, background_color);

    XSelectInput(m_display->xdisplay(), nonclient, SubstructureRedirectMask | SubstructureNotifyMask | ExposureMask | ResizeRedirectMask | EnterWindowMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask  | ButtonMotionMask | StructureNotifyMask );

    XAddToSaveSet(m_display->xdisplay(), w);
    
    XReparentWindow(m_display->xdisplay(), w, nonclient, 0, titlebar_size);
    XMapWindow(m_display->xdisplay(), nonclient);
 
    nonclients[w] = nonclient;
    clients[nonclient] = w;
    
    // Grab Alt-Tab 
    XGrabKey(m_display->xdisplay(), XKeysymToKeycode(m_display->xdisplay(), XK_Tab), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);
    // Grab Alt-F4
    XGrabKey(m_display->xdisplay(), XKeysymToKeycode(m_display->xdisplay(), XK_F4), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);
    // Grab left mouse button
    XGrabButton( m_display->xdisplay(), Button1, None, w, false, ButtonPressMask , GrabModeSync, GrabModeAsync, None, None);
}

void windowmanager::undecorate(const Window& w)
{
    if (nonclients.count(w)<1) return;
    const Window nonclient = nonclients[w];
    XUnmapWindow(m_display->xdisplay(), nonclient);
    XReparentWindow(m_display->xdisplay(), w, m_display->root(), 0, 0);
    XRemoveFromSaveSet(m_display->xdisplay(), w);
    XDestroyWindow(m_display->xdisplay(), nonclient);
    clients.erase( nonclients[w] );
    nonclients.erase(w);
}

const std::string windowmanager::last_error ()
{
    return "";
}