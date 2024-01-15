#include <input.hpp>

ludo::input::mouse ludo::input::s_mouse;
std::array<ludo::input::status, 2> ludo::input::s_key_status;

ludo::input::mouse::mouse() {}

void ludo::input::input::set_mouse(const ludo::input::mouse &_mouse)
{
    s_mouse = _mouse;
}

const ludo::input::mouse &ludo::input::input::get_mouse()
{
    return s_mouse;
}

void ludo::input::set_key(const ludo::input::key &_key, const ludo::input::status &_status)
{
    s_key_status[(size_t)_key] = _status;
}

const ludo::input::status &ludo::input::get_key(const ludo::input::key &_key)
{
    return s_key_status[(size_t)_key];
}