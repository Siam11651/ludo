#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <array>

namespace ludo
{
    class input
    {
    public:
        enum class key
        {
            mouse_left, mouse_right
        };

        enum class status
        {
            release, press, repeat
        };

        class mouse
        {
        private:
            glm::vec2 m_velocity;

        public:
            glm::vec2 position;

            mouse();
        };

    private:
        static ludo::input::mouse s_mouse;
        static std::array<ludo::input::status, 2> s_key_status;

    public:
        static void set_mouse(const ludo::input::mouse &_mouse);
        static const ludo::input::mouse &get_mouse();
        static void set_key(const ludo::input::key &_key, const ludo::input::status &_status);
        static const ludo::input::status &get_key(const ludo::input::key &_key);
    };
}

#endif