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

#ifndef reload_image_H
#define reload_image_H

#include "color.h"
#include <cstring>

namespace reload
{
    class image
    {
        public:

            void create(const unsigned& _width, const unsigned& _height)
            {
                width = _width;
                height = _height;
                buffersize = width*height*4;
                data = (uint8_t*)malloc(buffersize);
                for (size_t i=0; i<buffersize; i++) data[i]=255;
            }

            void clear(const color& _c)
            {
/*
                for (int x=0; x<width; x++)
                {
                    for (int y=0; y<height; y++)
                    {
                        set_pixel(x,y,_c);
                    }
                }
*/
		memset(data, 0, buffersize);
            }

            color& get_pixel(const int& _x, const int& _y, color& _c)
            {
                int at = (_x*4)+(((height-1)-_y)*width*4 );
                (*((int32_t*)_c.value)) = *((int32_t*)(data+at));
                return _c;
            }

            void set_pixel(const int& _x, const int& _y, const color& _c, 
                            const float& _alpha=1.0f)
            {
                color c;
                get_pixel(_x,_y, c);
                c.blend(_c, _alpha);
                int at = (_x*4)+(((height-1)-_y)*width*4);
                *((int32_t*)(data+at)) = *(int32_t*)c.value;
            }

            void blend (image* _img, const int& _xd, const int& _yd, const int& _xo,
                        const int& _yo, const int& _w, const int& _h, 
                         const float& _alpha=1.0f)
            {
                color c;
                int w = _w;
                int h = _h;
                if (w+_xd > width)  w -= (_xd+w)-width;
                if (h+_yd > height) h -= (_yd+h)-height;
                w = (w<0)?0:w;
                h = (h<0)?0:h;

                for (int x=0; x<w; x++)
                {
                    for (int y=0; y<h; y++)
                    {
                        _img->get_pixel(_xo+x, _yo+y, c);
                        set_pixel(_xd+x, _yd+y, c, _alpha);
                    }
                }

            }

	    void swap(image* _img)
	    {
		std::memcpy(data, _img->data, buffersize);
	    }

            size_t   buffersize;
            unsigned width;
            unsigned height;
            uint8_t* data;
    };
}
#endif //reload_image_H
