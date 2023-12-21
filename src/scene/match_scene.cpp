#include <scene/match_scene.hpp>

ludo::match_scene::match_scene() :
    scene()
{
    m_board_sprite.setup_sprite("assets/board.png");
    m_board.set_sprite_ptr(&m_board_sprite);
    m_camera.transform().position().z = 3.5f;

    m_gameobject_ptrs.insert(m_gameobject_ptrs.end(),
    {
        &m_board
    });
}

void ludo::match_scene::on_update()
{

}

void ludo::match_scene::on_late_update()
{

}