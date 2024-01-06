#include <screen.hpp>

size_t ludo::screen::window_width;
size_t ludo::screen::window_height;
std::string ludo::screen::window_title;

float ludo::screen::aspect_ratio()
{
    return (float)window_width / window_height;
}