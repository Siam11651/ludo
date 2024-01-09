#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <glm/glm.hpp>

namespace ludo
{
    class coin_object;

    class cell
    {
    public:
        size_t index;
        size_t block;
        bool safe;
        glm::vec3 position;
        std::vector<ludo::coin_object *> coin_ptrs;

        cell();
        cell(const size_t &_index, const size_t &_block, const glm::vec3 &_position);
        void place_coins();
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

        void constructor_helper(const size_t &_id);

    public:
        std::array<ludo::cell, 26> cells;

        block();
        block(const size_t &_id, const float &_scale, const ludo::block::color &_color);
        void rotate(const float &_angle);
    };

    class board
    {
    private:
        float m_scale;

        void constructor_helper();

    public:
        std::array<ludo::block, 4> blocks;

        board();
        board(const float &_scale);
        ludo::cell *get_next_cell_ptr(const size_t &_current_idx, const size_t &_block, const size_t &_owner, const size_t &_coin_idx);
    };
}

#endif