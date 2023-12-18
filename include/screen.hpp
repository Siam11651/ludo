#ifndef SCREEN_H
#define SCREEN_H

#include <cstddef>
#include <string>

class screen
{
private:
    static size_t s_window_width;
    static size_t s_window_height;
    static std::string s_window_title;

public:
    static size_t &window_width();
    static size_t &window_height();
    static std::string &window_title();
    static float aspect_ratio();
};

#endif