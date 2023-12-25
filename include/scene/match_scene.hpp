#ifndef MATCH_SCENE_H
#define MATCH_SCENE_H

#include <array>
#include <scene/scene.hpp>

namespace ludo
{
    class match_scene : public scene
    {
    private:
        std::array<ludo::sprite, 6> m_dice_sprites;
        ludo::sprite m_board_sprite;
        ludo::sprite m_coin_red_sprite;
        ludo::gameobject m_dice;
        ludo::gameobject m_board;
        ludo::gameobject m_coin_red;

    public:
        match_scene();
        void on_update();
        void on_late_update();
    };
}

#endif