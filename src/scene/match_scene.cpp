#include <sstream>
#include <iostream>
#include <scene/match_scene.hpp>
#include <animation.hpp>
#include <input.hpp>

ludo::match_scene::match_scene() :
    m_spinner_animation(&m_spinner),
    scene()
{
    m_dice_value = 1;

    for(size_t i = 0; i < 6; ++i)
    {
        std::stringstream ss;

        ss << "assets/dices/dice" << i + 1 << ".png";

        m_dice_sprites[i].setup_sprite(ss.str());
    }

    for(size_t i = 0; i < 15; ++i)
    {
        std::stringstream ss;

        ss << "assets/spinner/frame-" << i + 1 << ".png";

        m_spinner_sprites[i].setup_sprite(ss.str());

        ludo::keyframe new_keyframe(ludo::clk::duration(1000000000 / 30));

        new_keyframe.set_sprite_ptr(&m_spinner_sprites[i]);

        if(i == 14)
        {
            new_keyframe.on_reach() = [this]() -> void
            {
                this->m_dice.active() = true;
                this->m_spinner.active() = false;
                this->m_dice_value = this->rand_gen() % 6 + 1;
                this->m_dice.set_sprite_ptr(&this->m_dice_sprites[this->m_dice_value - 1]);
            };
        }

        m_spinner_animation.keyframes().push_back(new_keyframe);
    }

    m_spinner.active() = false;
    m_board_sprite.setup_sprite("assets/board.png");
    m_coin_red_sprite.setup_sprite("assets/coins/coin_red.png");
    m_dice.set_sprite_ptr(&m_dice_sprites.front());
    m_dice.local_transform().position().x = -0.89f;
    m_dice.local_transform().position().y = -1.11f;
    m_dice.local_transform().position().z = 0.01f;
    m_dice.local_transform().scale() /= 10.0f;
    m_spinner.local_transform().position().x = -0.89f;
    m_spinner.local_transform().position().y = -1.11f;
    m_spinner.local_transform().position().z = 0.01f;
    m_spinner.local_transform().scale() /= 10.0f;
    m_board.set_sprite_ptr(&m_board_sprite);
    m_coin_red.set_sprite_ptr(&m_coin_red_sprite);
    m_coin_red.local_transform().position().z = 0.01f;
    m_coin_red.local_transform().position().x = -2.0f / 15.0f;
    m_coin_red.local_transform().position().y = -12.0f / 15.0f;
    m_coin_red.local_transform().scale() /= 20.0f;
    m_camera.transform().position().z = 3.5f;

    m_gameobject_ptrs.insert(m_gameobject_ptrs.end(),
    {
        &m_board,
        &m_coin_red,
        &m_dice,
        &m_spinner
    });
}

void ludo::match_scene::on_update()
{
    const ludo::input::status current_status = ludo::input::get_key(ludo::input::key::mouse_left);

    if(m_previous_status == ludo::input::status::press
        && current_status == ludo::input::status::release)
    {
        const glm::vec2 &mouse_pos = ludo::input::get_mouse().const_position();

        if(40.0f <= mouse_pos.x && mouse_pos.x <= 92.0f
            && 825.0f <= mouse_pos.y && mouse_pos.y <= 877.0f)
        {
            m_dice.active() = false;
            m_spinner.active() = true;
            m_spinner_animation.play();
        }
    }

    m_previous_status = current_status;
}

void ludo::match_scene::on_late_update()
{

}