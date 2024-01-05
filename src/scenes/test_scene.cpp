#include <scenes/test_scene.hpp>
#include <sstream>

ludo::test_scene::test_scene()
{
    ludo::animation *meimei_animation = new ludo::animation(&m_meimei);

    for(size_t i = 0; i < 514; ++i)
    {
        std::stringstream ss;

        ss << "assets/meimei/" << i + 1 << ".png";

        m_sprites[i].setup_sprite(ss.str());
        
        ludo::keyframe new_keyframe(ludo::clk::duration(1000000000 / 24));
        
        new_keyframe.set_sprite_ptr(&m_sprites[i]);
        meimei_animation->keyframes.push_back(new_keyframe);
    }

    camera.transform.position.z = 3.5f;
    m_meimei.local_transform.scale.y = 9.0f / 16.0f;

    m_gameobject_ptrs.push_back(&m_meimei);

    meimei_animation->play();
}

void ludo::test_scene::on_update()
{

}

void ludo::test_scene::on_late_update()
{

}