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

#ifndef reload_color_H
#define reload_color_H

#include <iostream>

namespace reload
{
    // Class to handle colors in an image buffer. mostly to facilitate blending.
    class color
    {
        public:
            color (const uint8_t& _r=255, const uint8_t& _g=255, const uint8_t& _b=255, 
                    const uint8_t& _a=0)
            {
                set(_r,_g,_b,_a);
            }

            void set(const uint8_t& _r, const uint8_t& _g, const uint8_t& _b, const uint8_t& _a)
            {
                value[0]=_r; value[1]=_g; value[2]=_b; value[3]=_a;
            }

            void blend (const color& _c, const float& _alpha=1.0f)
            {
                unsigned int alpha  = _c.value[3]*_alpha+1;
                unsigned int ialpha = 256 - _c.value[3]*_alpha;

                value[0] = (uint8_t)((alpha*_c.value[0]+ialpha*value[0])>>8);
                value[1] = (uint8_t)((alpha*_c.value[1]+ialpha*value[1])>>8);
                value[2] = (uint8_t)((alpha*_c.value[2]+ialpha*value[2])>>8);
                value[3] = 255;
            }

            uint8_t value[4];
    };
}
#endif //reload_color_H
