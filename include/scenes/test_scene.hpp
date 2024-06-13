#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene.hpp>

namespace ludo
{
    class test_scene : public scene
    {
    private:
        ludo::sprite m_star_sprite;
        ludo::gameobject *m_star_gameobject;

    public:
        test_scene();
        void on_update();
        void on_late_update();
        ~test_scene();
    };
}

#endif