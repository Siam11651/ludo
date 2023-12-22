#include <scene/match_scene.hpp>

ludo::match_scene::match_scene() :
    scene()
{
    m_board_sprite.setup_sprite("assets/board.png");
    m_coin_red_sprite.setup_sprite("assets/coin_red.png");
    m_board.set_sprite_ptr(&m_board_sprite);
    m_coin_red.set_sprite_ptr(&m_coin_red_sprite);
    m_coin_red.local_transform().position().z = 0.01f;
    m_coin_red.local_transform().position().x = -1.0f + 4.0f / 15.0f;
    m_coin_red.local_transform().position().y = -1.0f + 4.0f / 15.0f;
    m_coin_red.local_transform().scale() /= 20.0f;
    m_camera.transform().position().z = 3.5f;

    m_gameobject_ptrs.insert(m_gameobject_ptrs.end(),
    {
        &m_board,
        &m_coin_red,
    });
}

void ludo::match_scene::on_update()
{

}

void ludo::match_scene::on_late_update()
{

}