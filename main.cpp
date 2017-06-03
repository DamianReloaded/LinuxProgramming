#include "window.h"
#include <sys/time.h>
#include <math.h>

owl::display display;
owl::window  window;
owl::bitmap  bitmap;
owl::bitmap  background;
owl::bitmap  animation;
int frame = 0;
double last_ticks = 0;

double get_ticks()
{
    time_t s;
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec); // CLOCK_MONOTONIC seems to be important.
    s = spec.tv_sec;
    return round(spec.tv_nsec/1.0e6);
}

// Callback for the expose event
void on_expose(owl::window* _w)
{
    window.blend(&background,0,0,0,0,640,480);
    window.blend(&bitmap,195,115,0,0,250,250);
    window.blend(&animation,510,20,frame,0,128,128);
}

// Application Entry Point
int main()
{
    if (!display.open()) return -1;
    window.create(0,0,640,480,&display);
    window.on_expose = owl::window::event(on_expose);
    bitmap.display(&display);
    background.display(&display);
    animation.display(&display);
    bitmap.load("yingyang2.png");
    background.load("mrain.png");
    animation.load("mushy.png");

    // Main Loop
    while(window.running())
    {
        window.update();

        // Update animation every 150 milliseconds
        if (fabs(get_ticks()-last_ticks)>150) // for some reason I get negative values here.
        {
            frame +=128; // this is the offset in the PNG, which are 4 images of 128 pixels width
            if (frame>=512) frame=0; // after the fourth frame we start over.
            last_ticks = get_ticks();
        }
    }
    
    /*
    bitmap.destroy();
    background.destroy();
    animation.destroy();
    window.destroy();
    display.close();
    */
    return 0;
}
