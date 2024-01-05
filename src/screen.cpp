#include <screen.hpp>

size_t ludo::screen::s_window_width;
size_t ludo::screen::s_window_height;
std::string ludo::screen::s_window_title;

size_t &ludo::screen::window_width()
{
    return s_window_width;
}

size_t &ludo::screen::window_height()
{
    return s_window_height;
}

std::string &ludo::screen::window_title()
{
    return s_window_title;
}

float ludo::screen::aspect_ratio()
{
    return (float)s_window_width / s_window_height;
}