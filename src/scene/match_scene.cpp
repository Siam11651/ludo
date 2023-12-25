#include <sstream>
#include <iostream>
#include <scene/match_scene.hpp>
#include <animation.hpp>
#include <input.hpp>

ludo::match_scene::match_scene() :
    scene()
{
    m_dice_value = 1;

    for(size_t i = 0; i < 6; ++i)
    {
        std::stringstream ss;

        ss << "assets/dices/dice" << i + 1 << ".png";

        m_dice_sprites[i].setup_sprite(ss.str());
    }

    for(size_t i = 0; i < 4; ++i)
    {
        m_spinner_animations[i] = new ludo::animation(&m_spinners[i]);
    }

    for(size_t i = 0; i < 15; ++i)
    {
        std::stringstream ss;

        ss << "assets/spinner/frame-" << i + 1 << ".png";

        m_spinner_sprites[i].setup_sprite(ss.str());

        ludo::keyframe new_keyframe(ludo::clk::duration(1000000000 / 30));

        new_keyframe.set_sprite_ptr(&m_spinner_sprites[i]);

        if(i + 1 == m_spinner_sprites.size())
        {
            for(size_t k = 0; k < 4; ++k)
            {
                new_keyframe.on_reach() = [this, k]() -> void
                {
                    if(this->m_moves[k].size() == 3)
                    {
                        this->m_moves[k].clear();
                    }

                    if(this->m_moves[k].size() == 0 || this->m_moves[k].back() < 6)
                    {
                        for(size_t j = 0; j < 3; ++j)
                        {
                            this->m_streak_dices[j].active() = false;
                        }
                    }

                    this->m_dice.active() = true;
                    this->m_spinners[k].active() = false;
                    this->m_dice_value = this->rand_gen() % 6 + 1;

                    this->m_dice.set_sprite_ptr(&this->m_dice_sprites[this->m_dice_value - 1]);

                    this->m_streak_dices[this->m_moves[k].size()].active() = true;
                    this->m_streak_dices[this->m_moves[k].size()]
                        .set_sprite_ptr(&this->m_dice_sprites[this->m_dice_value - 1]);

                    if(this->m_moves[k].size() > 0 && this->m_moves[k].back() == 6)
                    {
                        this->m_moves[k].push_back(this->m_dice_value);
                    }
                    else
                    {
                        this->m_moves[k] = {this->m_dice_value};
                    }

                    for(size_t j = 0; j < 3; ++j)
                    {
                        this->m_streak_dices[j].active() = false;
                    }

                    for(size_t j = 0; j < this->m_moves[k].size(); ++j)
                    {
                        this->m_streak_dices[j].active() = true;

                        this->m_streak_dices[j].set_sprite_ptr(
                            &this->m_dice_sprites[this->m_moves[k][j] - 1]);
                    }

                    if(this->m_moves[k].size() == 3 && this->m_moves[k].back() == 6)
                    {
                        this->m_moves[k].clear();
                    }
                };

                m_spinner_animations[k]->keyframes().push_back(new_keyframe);
            }
        }
        else
        {
            for(size_t k = 0; k < 4; ++k)
            {
                m_spinner_animations[k]->keyframes().push_back(new_keyframe);
            }
        }
    }

    for(size_t i = 0; i < 3; ++i)
    {
        m_streak_dices[i].local_transform().position() =
            glm::vec3(-0.9f + i * 0.15f, -1.3f, 0.01f);
        m_streak_dices[i].local_transform().scale() /= 15.0f;

        m_streak_dices[i].set_sprite_ptr(&m_dice_sprites[0]);
        m_streak_dices[i].active() = false;
    }

    m_curren_cell_ptr = &m_board_handler.const_blocks().front().const_cells()[7];

    for(size_t i = 0; i < 4; ++i)
    {
        const glm::mat4x4 rotation_mat = glm::rotate(glm::identity<glm::mat4x4>(),
            (float)(M_PI * i) / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::vec3 new_position = glm::vec3(rotation_mat
            * glm::vec4(-0.89f, -1.11f, 0.01f, 1.0f));
        m_spinners[i].active() = false;
        m_spinners[i].local_transform().position() = new_position;
        m_spinners[i].local_transform().scale() /= 10.0f;
    }

    m_board_sprite.setup_sprite("assets/board.png");
    m_coin_red_sprite.setup_sprite("assets/coins/coin_red.png");
    m_dice.set_sprite_ptr(&m_dice_sprites.front());
    m_dice.local_transform().position() = glm::vec3(-0.89f, -1.11f, 0.01f);
    m_dice.local_transform().scale() /= 10.0f;
    m_board.set_sprite_ptr(&m_board_sprite);
    m_coin_red.set_sprite_ptr(&m_coin_red_sprite);
    m_coin_red.local_transform().position() = m_curren_cell_ptr->const_position();
    m_coin_red.local_transform().position().z = 0.01f;
    m_coin_red.local_transform().scale() /= 20.0f;
    m_camera.transform().position().z = 3.5f;

    m_gameobject_ptrs.insert(m_gameobject_ptrs.end(),
    {
        &m_board,
        &m_coin_red,
        &m_dice,
    });

    for(size_t i = 0; i < 4; ++i)
    {
        m_gameobject_ptrs.push_back(&m_spinners[i]);
    }

    for(size_t i = 0; i < 3; ++i)
    {
        m_gameobject_ptrs.push_back(&m_streak_dices[i]);
    }
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
            m_spinners[0].active() = true;
            m_spinner_animations[0]->play();
        }
    }

    m_previous_status = current_status;
}

void ludo::match_scene::on_late_update()
{

}