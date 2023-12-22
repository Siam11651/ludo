#ifndef MATCH_SCENE_H
#define MATCH_SCENE_H

#include <scene/scene.hpp>

namespace ludo
{
    class match_scene : public scene
    {
    private:
        ludo::sprite m_board_sprite;
        ludo::sprite m_coin_red_sprite;
        ludo::gameobject m_board;
        ludo::gameobject m_coin_red;

    public:
        match_scene();
        void on_update();
        void on_late_update();
    };
}

#endif