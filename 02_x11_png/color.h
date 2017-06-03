#include <iostream>

namespace owl
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

            void blend (const color& _c)
            {
                unsigned int alpha  = _c.value[3]+1;
                unsigned int ialpha = 256 - _c.value[3];

                value[0] = (uint8_t)((alpha*_c.value[0]+ialpha*value[0])>>8);
                value[1] = (uint8_t)((alpha*_c.value[1]+ialpha*value[1])>>8);
                value[2] = (uint8_t)((alpha*_c.value[2]+ialpha*value[2])>>8);
                value[3] = 255;
            }

            uint8_t value[4];
    };
}
