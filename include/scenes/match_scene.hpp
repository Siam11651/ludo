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
    class coin_object : public gameobject
    {
    private:
        ludo::cell *m_current_cell_ptr;

    public:
        coin_object();
        void set_current_cell_ptr(ludo::cell *_current_cell_ptr);
        ludo::cell *get_current_cell_ptr() const;
    };

    class dice_button : public button
    {
    public:
        uint8_t value;
        uint8_t coin;

        dice_button();
    };

    class match_scene : public scene
    {
    private:
        bool m_move;
        uint8_t m_turn;
        size_t m_player_count;
        ludo::input::status m_previous_mouse_status;
        std::array<uint8_t, 4> m_dice_values;
        std::array<std::vector<uint8_t>, 4> m_moves;
        ludo::board m_board_handler;
        const ludo::cell *m_curren_cell_ptr;
        ludo::input::status m_previous_status;
        std::array<ludo::sprite, 6> m_dice_sprites;
        std::array<ludo::sprite, 6> m_act_dice_sprites;
        std::array<ludo::sprite, 15> m_spinner_sprites;
        ludo::sprite m_board_sprite;
        std::array<ludo::sprite, 4> m_coin_sprites;
        std::array<ludo::animation *, 4> m_spinner_animations;
        std::array<ludo::button, 4> m_dices;
        std::array<std::array<ludo::gameobject, 3>, 4> m_streak_dices;
        std::array<std::array<ludo::dice_button, 3>, 4> m_input_dices;
        ludo::gameobject m_board;
        std::array<ludo::gameobject, 4> m_spinners;
        std::array<std::array<ludo::coin_object, 4>, 4> m_coins;

        void change_turn();
        void disable_input_dices();

    public:
        match_scene();
        void on_update();
        void on_late_update();
    };
}

#endif