#ifndef MATCH_SCENE_H
#define MATCH_SCENE_H

#include <random>
#include <array>
#include <input.hpp>
#include <scene.hpp>
#include <animation.hpp>
#include <board.hpp>

namespace ludo
{
    class match_scene : public scene
    {
    private:
        uint8_t m_turn;
        size_t m_player_count;
        std::random_device rand_gen;
        std::array<uint8_t, 4> m_dice_values;
        std::array<std::vector<uint8_t>, 4> m_moves;
        ludo::board m_board_handler;
        const ludo::cell *m_curren_cell_ptr;
        ludo::input::status m_previous_status;
        std::array<float, 4> m_button_tops;
        std::array<float, 4> m_button_bots;
        std::array<float, 4> m_button_lefts;
        std::array<float, 4> m_button_rights;
        std::array<ludo::sprite, 6> m_dice_sprites;
        std::array<ludo::sprite, 6> m_act_dice_sprites;
        std::array<ludo::sprite, 15> m_spinner_sprites;
        ludo::sprite m_board_sprite;
        ludo::sprite m_coin_red_sprite;
        std::array<ludo::animation *, 4> m_spinner_animations;
        std::array<ludo::gameobject, 4> m_dices;
        std::array<std::array<ludo::gameobject, 3>, 4> m_streak_dices;
        ludo::gameobject m_board;
        std::array<ludo::gameobject, 4> m_spinners;
        ludo::gameobject m_coin_red;

    public:
        match_scene();
        void on_update();
        void on_late_update();
    };
}

#endif