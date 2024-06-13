#include <scenes/test_scene.hpp>

ludo::test_scene::test_scene() : scene()
{
    m_star_sprite.setup_sprite("assets/star.png");

    m_star_gameobject = new ludo::gameobject(this);
    main_camera.transform.position.z = 3.5f;

    m_star_gameobject->sprite_ptr = &m_star_sprite;
    m_world_element_ptrs.push_back(m_star_gameobject);
}

void ludo::test_scene::on_update()
{

}

void ludo::test_scene::on_late_update()
{

}

ludo::test_scene::~test_scene()
{
    delete m_star_gameobject;
}