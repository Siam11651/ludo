#ifndef SCREEN_H
#define SCREEN_H

#include <cstddef>
#include <string>

namespace ludo
{
    class screen
    {
    public:
        static size_t window_width;
        static size_t window_height;
        static std::string window_title;
        
        static float aspect_ratio();
    };   
}

#endif