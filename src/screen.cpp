#include <screen.hpp>

size_t screen::s_window_width;
size_t screen::s_window_height;
std::string screen::s_window_title;

size_t &screen::window_width()
{
    return s_window_width;
}

size_t &screen::window_height()
{
    return s_window_height;
}

std::string &screen::window_title()
{
    return s_window_title;
}

float screen::aspect_ratio()
{
    return (float)s_window_width / s_window_height;
}