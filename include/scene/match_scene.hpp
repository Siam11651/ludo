#ifndef MATCH_SCENE_H
#define MATCH_SCENE_H

#include <random>
#include <array>
#include <input.hpp>
#include <scene/scene.hpp>
#include <animation.hpp>

namespace ludo
{
    class match_scene : public scene
    {
    private:
        std::random_device rand_gen;
        uint8_t m_dice_value;
        ludo::input::status m_previous_status;
        std::array<ludo::sprite, 6> m_dice_sprites;
        std::array<ludo::sprite, 15> m_spinner_sprites;
        ludo::sprite m_board_sprite;
        ludo::sprite m_coin_red_sprite;
        ludo::animation m_spinner_animation;
        ludo::gameobject m_dice;
        ludo::gameobject m_board;
        ludo::gameobject m_spinner;
        ludo::gameobject m_coin_red;

    public:
        match_scene();
        void on_update();
        void on_late_update();
    };
}

#endif