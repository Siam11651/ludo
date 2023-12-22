#include <scene/match_scene.hpp>
#include <animation.hpp>

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
    ludo::keyframe move0(ludo::clk::duration(1000000000));
    ludo::keyframe move1(ludo::clk::duration(1000000000));
    ludo::keyframe move2;
    move0.transform_opt() = m_coin_red.const_local_transform();
    move1.transform_opt() = m_coin_red.const_local_transform();
    move1.transform_opt().value().position().x = -1.0f + 16.0f / 15.0f;
    move2.transform_opt() = move1.transform_opt().value();
    move2.transform_opt().value().position().y = -1.0f + 16.0f / 15.0f;
    ludo::animation *move = new ludo::animation(&m_coin_red);

    move->keyframes().insert(move->keyframes().end(),
    {
        move0,
        move1,
        move2
    });
    move->play();
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