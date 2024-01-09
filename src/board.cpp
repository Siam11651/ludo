#include <board.hpp>
#include <scenes/match_scene.hpp>
#include <glm/ext.hpp>

ludo::cell::cell() :
    safe(false),
    index(SIZE_MAX) {}

ludo::cell::cell(const size_t &_index, const size_t &_block, const glm::vec3 &_position) :
    safe(false),
    index(_index),
    block(_block),
    position(_position) {}

void ludo::cell::place_coins()
{
    constexpr float x_gap = 0.015f;
    const float length = x_gap * (coin_ptrs.size() - 1);
    const float offset = -length / 2.0f;

    for(size_t i = 0; i < coin_ptrs.size(); ++i)
    {
        glm::vec3 new_position = position;
        new_position.x += offset + x_gap * i;
        new_position.z += 0.001f * i;
        coin_ptrs[i]->local_transform.position = new_position;
    }
}

void ludo::block::constructor_helper(const size_t &_id)
{
    for(size_t i = 0; i < cells.size(); ++i)
    {
        cells[i].index = i;
        cells[i].block = _id;
    }

    for(size_t i = 0; i < 6; ++i)
    {
        const float var_pos = (-4.0f / 15.0f - (2.0f * i) / 15.0f) * m_scale;
        const float fix_pos = (-2.0f / 15.0f) * m_scale;
        cells[i].position = glm::vec3(0.0f, var_pos, 0.0f);

        if(i < 5)
        {
            cells[i].safe = true;
        }

        cells[11 - i].position.y = var_pos;
        cells[11 - i].position.x = fix_pos;
        cells[12 + i].position.x = var_pos;
        cells[12 + i].position.y = fix_pos;
    }

    cells[7].safe = true;
    cells[15].safe = true;
    const float home_pos[2] = {(-7.0f / 15.0f) * m_scale, (-11.0f / 15.0f) * m_scale};
    cells[18].position.x = home_pos[1];
    cells[18].position.y = home_pos[1];
    cells[19].position.x = home_pos[0];
    cells[19].position.y = home_pos[1];
    cells[20].position.x = home_pos[0];
    cells[20].position.y = home_pos[0];
    cells[21].position.x = home_pos[1];
    cells[21].position.y = home_pos[0];
    cells[22].position = glm::vec3(0.0f, -0.1f, 0.0f);
    cells[23].position = glm::vec3(0.0f, -0.15f, 0.0f);
    cells[24].position = glm::vec3(-0.075f, -0.15f, 0.0f);
    cells[25].position = glm::vec3(0.075f, -0.15f, 0.0f);

    for(size_t i = 18; i < 22; ++i)
    {
        cells[i].safe = true;
        cells[i + 4].safe = true;
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
    constructor_helper(-1);
}

ludo::block::block(const size_t &_id, const float &_scale, const ludo::block::color &_color) :
    m_scale(_scale),
    m_color(_color)
{
    constructor_helper(_id);
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
        blocks[i] = ludo::block(i, m_scale, (ludo::block::color)i);

        blocks[i].rotate(i * M_PI / 2.0f);
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

ludo::cell *ludo::board::get_next_cell_ptr(const size_t &_current_idx, const size_t &_current_block, const size_t &_owner, const size_t &_coin_idx)
{
    if(_current_idx == 0)
    {
        return &blocks[_current_block].cells[22 + _coin_idx];
    }
    else if(_current_idx < 5)
    {
        return &blocks[_current_block].cells[_current_idx - 1];
    }
    else if(_current_idx == 5)
    {
        if(_current_block == _owner)
        {
            return &blocks[_current_block].cells[4];
        }
        else
        {
            return &blocks[_current_block].cells[6];
        }
    }
    else if(_current_idx < 17)
    {
        return &blocks[_current_block].cells[_current_idx + 1];
    }
    else if(_current_idx == 17)
    {
        return &blocks[(_current_block - 1) % 4].cells[5];
    }
    else if(_current_idx < 22)
    {
        return &blocks[_current_block].cells[7];
    }

    return nullptr;
}