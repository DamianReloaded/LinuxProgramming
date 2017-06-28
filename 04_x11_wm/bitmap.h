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
   in a product, an acknreloadedgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef reload_bitmap_H
#define reload_bitmap_H

#include "display.h"
#include "image.h"
#include "lodepng.h"
#include <string>

namespace reload
{
    class window;

    // Class to pass image buffer data to X11
    class bitmap
    {
        public:
            bitmap (reload::display* _display=NULL) : m_display(_display), m_ximage(0) {}
            virtual ~bitmap()
            {
                destroy();
            }
            
            void display(reload::display* _display) { m_display = _display; }

            void create(const unsigned& _width, const unsigned& _height)
            {
                // We create a image buffer
                destroy();
                m_image.create(_width,_height);
                // And we associate that buffer with an ximage that we can draw on X11 drawables
                m_ximage =  XCreateImage(m_display->m_display, m_display->m_visual,24,ZPixmap,0,
                                (char*)m_image.data, m_image.width, m_image.height,32,0);
            }

            // Load a PNG file using LodePNG (great handy lib)
            const bool load(const std::string& _filepath)
            {
                if (m_display==NULL) return false;
                destroy();
                unsigned error = lodepng_decode32_file(&m_image.data, &m_image.width, 
                                    &m_image.height,_filepath.c_str());
                if (error!=0)
                {
                    return false;
                }

                size_t size = m_image.width*m_image.height*4;
                uint8_t tmp = 0;
                for (size_t i=0; i<size; i+=4)
                {
                    tmp = m_image.data[i];
                    m_image.data[i] = m_image.data[i+2];
                    m_image.data[i+2] = tmp;
                }

                m_ximage = XCreateImage(m_display->m_display, m_display->m_visual,24,ZPixmap,0,
                                (char*)&m_image.data[0],m_image.width, m_image.height,32,0);
                return true;
            }

            void destroy()
            {
                if (m_ximage!=0) XDestroyImage(m_ximage);
                m_ximage=0;
            }

            void blend(bitmap* bmp, const int& _xd, const int& _yd, const int& _xo, const int& _yo,
                            const int& _w, const int& _h, const float& _alpha=1.0f)
            {
                m_image.blend(&bmp->m_image, _xd, _yd, _xo, _yo, _w, _h, _alpha);
            }

            void clear(const color& _c)
            {
                m_image.clear(_c);
            }

            const int width() { return m_image.width; }
            const int height() { return m_image.height; }

        protected:
            reload::image      m_image;
            reload::display*   m_display;
            XImage*            m_ximage;

        friend class window;
    };
}
#endif //reload_bitmap_H
