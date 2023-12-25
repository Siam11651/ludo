#include <board.hpp>

ludo::cell::cell() :
    m_index(SIZE_MAX) {}

ludo::cell::cell(const size_t &_index, const glm::vec3 &_position) :
    m_index(_index),
    m_position(_position) {}

uint8_t &ludo::cell::safety()
{
    return m_safety;
}

const uint8_t &ludo::cell::const_safety() const
{
    return m_safety;
}

size_t &ludo::cell::index()
{
    return m_index;
}

const size_t &ludo::cell::const_index() const
{
    return m_index;
}

glm::vec3 &ludo::cell::position()
{
    return m_position;
}

const glm::vec3 &ludo::cell::const_position() const
{
    return m_position;
}

void ludo::block::constructor_helper()
{
    for(size_t i = 0; i < 6; ++i)
    {
        const float var_pos = (-2.0f / 15.0f - i / 15.0f) * m_scale;
        const float fix_pos = (-1.0f / 15.0f) * m_scale;
        m_cells[i].position().y = var_pos;
        m_cells[i].safety() = 1;
        m_cells[11 - i].position().y = var_pos;
        m_cells[11 - i].position().x = fix_pos;
        m_cells[12 + i].position().x = var_pos;
        m_cells[12 + i].position().y = fix_pos;
    }

    m_cells[7].safety() = 2;
    m_cells[15].safety() = 2;
    const float home_pos[2] = {(-3.5f / 15.0f) * m_scale, (-5.5f / 15.0f) * m_scale};
    m_cells[18].position().x = home_pos[1];
    m_cells[18].position().y = home_pos[1];
    m_cells[19].position().x = home_pos[0];
    m_cells[19].position().y = home_pos[1];
    m_cells[20].position().x = home_pos[0];
    m_cells[20].position().y = home_pos[0];
    m_cells[21].position().x = home_pos[1];
    m_cells[21].position().y = home_pos[0];

    for(size_t i = 18; i < 22; ++i)
    {
        m_cells[i].safety() = 1;
    }
}

ludo::block::block() :
    m_scale(1.0f),
    m_color(ludo::block::color::red)
{
    constructor_helper();
}

ludo::block::block(const float &_scale, const ludo::block::color &_color) :
    m_scale(_scale),
    m_color(_color)
{
    constructor_helper();
}