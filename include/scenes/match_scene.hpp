#ifndef MATCH_SCENE_H
#define MATCH_SCENE_H

#include <random>
#include <array>
#include <queue>
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
        bool finished;

        coin_object(ludo::scene *_scene);
        void set_current_cell_ptr(ludo::cell *_current_cell_ptr);
        ludo::cell *get_current_cell_ptr() const;
    };

    class dice_button : public button
    {
    public:
        uint8_t value;
        uint8_t coin;

        dice_button(ludo::scene *_scene);
    };

    class match_scene : public scene
    {
    private:
        const std::chrono::nanoseconds m_coin_move_duration;
        /**
         * @brief state variable which indicates if right now we are to select select a coin to finish a move or not
         * 
         */
        bool m_move;
        /**
         * @brief state variable denoting if coin animating or not
         * 
         */
        bool m_coin_animating;
        /**
         * @brief index to player to make the current dice roll
         * 
         */
        uint8_t m_turn;
        /**
         * @brief number of players in the current match
         * 
         */
        size_t m_player_count;
        ludo::input::status m_previous_mouse_status;
        /**
         * @brief dice values of all the 4 players
         * 
         */
        std::array<uint8_t, 4> m_dice_values;
        /**
         * @brief move streak count of player with control of dice
         * 
         */
        std::array<size_t, 4> m_move_streaks;
        /**
         * @brief moves in the streak of player with control of dice
         * 
         */
        std::array<std::vector<uint8_t>, 4> m_moves;
        ludo::board m_board_handler;
        const ludo::cell *m_curren_cell_ptr;
        ludo::input::status m_previous_status;
        std::array<ludo::sprite, 6> m_dice_sprites;
        std::array<ludo::sprite, 6> m_semi_act_dice_sprites;
        std::array<ludo::sprite, 6> m_act_dice_sprites;
        std::array<ludo::sprite, 15> m_spinner_sprites;
        ludo::sprite m_board_sprite;
        std::array<ludo::sprite, 4> m_coin_sprites;
        std::array<ludo::animation *, 4> m_spinner_animations;
        std::array<ludo::button *, 4> m_dices;
        std::array<ludo::gameobject *, 4> m_streak_dices_holder;
        std::array<std::array<ludo::gameobject *, 10>, 4> m_streak_dices;
        std::array<std::array<ludo::dice_button *, 10>, 4> m_input_dices;
        ludo::gameobject *m_board;
        std::array<ludo::gameobject *, 4> m_spinners;
        std::array<std::array<uint8_t, 4>, 4> m_finished_coin_count;
        std::array<std::array<ludo::coin_object *, 4>, 4> m_coins;
        std::array<std::array<ludo::animation *, 4>, 4> m_coin_animations;
        std::queue<ludo::animation *> m_animations_cleaner;

        void change_turn(const bool _bonus = false);
        void disable_input_dices();
        /**
         * @brief method to make the final move and keep the coin in place
         * 
         * @param _value number of cells to cross
         * @param _coin index of the coin to move
         * @param _remove value to remove from streak (defaults to that of _value)
         */
        void make_move(const uint8_t &_value, const uint8_t &_coin, const bool &_start = false);

    public:
        match_scene();
        void on_update();
        void on_late_update();
        ~match_scene();
    };
}

#endif