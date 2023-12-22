#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene/scene.hpp>
#include <animation.hpp>
#include <array>

namespace ludo
{
    class test_scene : public scene
    {
    private:
        

    public:
        test_scene();
        void on_update();
        void on_late_update();
    };
}

#endif