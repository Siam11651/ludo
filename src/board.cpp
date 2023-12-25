#include <board.hpp>
#include <glm/ext.hpp>

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
        const float var_pos = (-4.0f / 15.0f - (2.0f * i) / 15.0f) * m_scale;
        const float fix_pos = (-2.0f / 15.0f) * m_scale;
        m_cells[i].position().y = var_pos;

        if(i < 5)
        {
            m_cells[i].safety() = 1;
        }

        m_cells[11 - i].position().y = var_pos;
        m_cells[11 - i].position().x = fix_pos;
        m_cells[12 + i].position().x = var_pos;
        m_cells[12 + i].position().y = fix_pos;
    }

    m_cells[7].safety() = 2;
    m_cells[15].safety() = 2;
    const float home_pos[2] = {(-7.0f / 15.0f) * m_scale, (-11.0f / 15.0f) * m_scale};
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

void ludo::block::rotate(const float &_angle)
{
    const glm::mat4x4 rotation_mat = glm::rotate(glm::identity<glm::mat4x4>(), _angle, glm::vec3(0.0f, 0.0f, 1.0f));

    for(size_t i = 0; i < m_cells.size(); ++i)
    {
        const glm::vec4 new_position = rotation_mat
            * glm::vec4(m_cells[i].const_position(), 1.0f);
        m_cells[i].position() = glm::vec3(new_position);
    }
}

std::array<ludo::cell, 22> &ludo::block::cells()
{
    return m_cells;
}

const std::array<ludo::cell, 22> &ludo::block::const_cells() const
{
    return m_cells;
}

void ludo::board::constructor_helper()
{
    for(size_t i = 0; i < 4; ++i)
    {
        m_blocks[i] = ludo::block(m_scale, (ludo::block::color)i);

        m_blocks[i].rotate(i * M_PI / 2.0f);
    }
}

ludo::board::board() :
    m_scale(1.0f)
{
    constructor_helper();
}

ludo::board::board(const float &_scale) :
    m_scale(_scale)
{
    constructor_helper();
}

std::array<ludo::block, 4> &ludo::board::blocks()
{
    return m_blocks;
}

const std::array<ludo::block, 4> &ludo::board::const_blocks() const
{
    return m_blocks;
}