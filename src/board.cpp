#include <board.hpp>
#include <glm/ext.hpp>

ludo::cell::cell() :
    next_ptr(nullptr),
    index(SIZE_MAX) {}

ludo::cell::cell(const size_t &_index, const glm::vec3 &_position) :
    next_ptr(nullptr),
    index(_index),
    position(_position) {}

void ludo::block::constructor_helper()
{
    for(size_t i = 0; i < 6; ++i)
    {
        const float var_pos = (-4.0f / 15.0f - (2.0f * i) / 15.0f) * m_scale;
        const float fix_pos = (-2.0f / 15.0f) * m_scale;
        cells[i].position.y = var_pos;

        if(i < 5)
        {
            cells[i].safety = 1;
        }

        cells[11 - i].position.y = var_pos;
        cells[11 - i].position.x = fix_pos;
        cells[12 + i].position.x = var_pos;
        cells[12 + i].position.y = fix_pos;
    }

    cells[7].safety = 2;
    cells[15].safety = 2;
    const float home_pos[2] = {(-7.0f / 15.0f) * m_scale, (-11.0f / 15.0f) * m_scale};
    cells[18].position.x = home_pos[1];
    cells[18].position.y = home_pos[1];
    cells[19].position.x = home_pos[0];
    cells[19].position.y = home_pos[1];
    cells[20].position.x = home_pos[0];
    cells[20].position.y = home_pos[0];
    cells[21].position.x = home_pos[1];
    cells[21].position.y = home_pos[0];

    for(size_t i = 18; i < 22; ++i)
    {
        cells[i].safety = 1;
    }

    for(ludo::cell &cell : cells)
    {
        cell.position.z = 0.01f;
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

    for(size_t i = 0; i < cells.size(); ++i)
    {
        const glm::vec4 new_position = rotation_mat * glm::vec4(cells[i].position, 1.0f);
        cells[i].position = glm::vec3(new_position);
    }
}

void ludo::board::constructor_helper()
{
    for(size_t i = 0; i < 4; ++i)
    {
        blocks[i] = ludo::block(m_scale, (ludo::block::color)i);

        blocks[i].rotate(i * M_PI / 2.0f);

        for(size_t j = 5; j < 17; ++j)
        {
            blocks[i].cells[j].next_ptr = &blocks[i].cells[j + 1];
        }

        blocks[i].cells[17].next_ptr = &blocks[(i - 1) % 4].cells[5];

        for(size_t j = 18; j < 22; ++j)
        {
            blocks[i].cells[j].next_ptr = &blocks[i].cells[7];
        }
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