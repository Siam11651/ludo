#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <glm/glm.hpp>

namespace ludo
{
    class cell
    {
    private:
        size_t m_index;
        uint8_t m_safety;
        glm::vec3 m_position;
    
    public:
        cell();
        cell(const size_t &_index, const glm::vec3 &_position);
        uint8_t &safety();
        const uint8_t &const_safety() const;
        size_t &index();
        const size_t &const_index() const;
        glm::vec3 &position();
        const glm::vec3 &const_position() const;
    };

    class block
    {
    public:
        enum class color
        {
            red, blue, yellow, green
        };

    private:
        float m_scale;
        ludo::block::color m_color;
        std::array<ludo::cell, 22> m_cells;

        void constructor_helper();

    public:
        block();
        block(const float &_scale, const ludo::block::color &_color);
    };

    class board
    {
    
    };
}

#endif