#include <sstream>
#include <screen.hpp>
#include <scenes/match_scene.hpp>
#include <animation.hpp>
#include <glm/gtx/quaternion.hpp>
#include <input.hpp>
#include <iostream>

ludo::match_scene::match_scene() :
    scene()
{
    m_move = false;
    m_player_count = 2;
    m_turn = 0;

    for(size_t i = 0; i < 6; ++i)
    {
        std::stringstream ss;

        ss << "assets/dices/dice" << i + 1 << ".png";

        m_dice_sprites[i].setup_sprite(ss.str());

        ss.str("");

        ss << "assets/act-dices/dice" << i + 1 << ".png";

        m_act_dice_sprites[i].setup_sprite(ss.str());
    }

    for(size_t i = 0; i < 4; ++i)
    {
        m_dice_values[i] = 1;
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
                new_keyframe.on_reach = [this, k]() -> void
                {
                    m_dices[k].active = true;
                    m_spinners[k].active = false;
                    m_dice_values[k] = std::random_device()() % 6 + 1;
                    m_streak_dices[k][m_moves[k].size()].active = true;

                    if(m_moves[k].size() > 0 && m_moves[k].back() == 6)
                    {
                        m_moves[k].push_back(m_dice_values[k]);
                    }
                    else
                    {
                        m_moves[k] = {m_dice_values[k]};
                    }

                    for(size_t j = 0; j < 3; ++j)
                    {
                        if(j < m_moves[k].size())
                        {
                            m_streak_dices[k][j].active = true;

                            m_streak_dices[k][j].set_sprite_ptr(
                                &m_act_dice_sprites[m_moves[k][j] - 1]);
                        }
                        else
                        {
                            m_streak_dices[k][j].active = false;
                        }
                    }

                    if(m_moves[k].size() == 3)
                    {
                        if(m_moves[k].back() < 6)
                        {
                            m_move = true;
                        }

                        m_moves[k].clear();
                    }
                    else if(m_moves[k].back() < 6)
                    {
                        m_move = true;
                    }

                    m_dices[k].set_sprite_ptr(&m_dice_sprites[m_dice_values[k] - 1]);

                    if(!m_move)
                    {
                        m_dices[m_turn].set_sprite_ptr(&m_act_dice_sprites[m_dice_values[m_turn] - 1]);
                    }
                };

                m_spinner_animations[k]->keyframes.push_back(new_keyframe);
            }
        }
        else
        {
            for(size_t k = 0; k < 4; ++k)
            {
                m_spinner_animations[k]->keyframes.push_back(new_keyframe);
            }
        }
    }

    m_curren_cell_ptr = &m_board_handler.const_blocks().front().const_cells()[7];

    for(size_t i = 0; i < 4; ++i)
    {
        const glm::mat4 rotation_mat = glm::rotate(glm::identity<glm::mat4>(),
            (float)(M_PI * i) / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 new_position = glm::vec3(rotation_mat
            * glm::vec4(-0.8f, -1.04f, 0.0f, 1.0f));
        m_spinners[i].active = false;
        m_spinners[i].local_transform.position = new_position;
        m_spinners[i].local_transform.scale /= 5.0f;
        m_dices[i].local_transform.position = new_position;
        m_dices[i].local_transform.scale /= 5.0f;

        if(i % m_player_count != 0)
        {
            m_dices[i].active = false;
        }

        if(i == 0)
        {
            m_dices[i].set_sprite_ptr(&m_act_dice_sprites.front());
        }
        else
        {
            m_dices[i].set_sprite_ptr(&m_dice_sprites.front());
        }

        for(size_t j = 0; j < 3; ++j)
        {
            new_position = glm::vec3(rotation_mat
                * glm::vec4(-0.82f + j * 0.15f, -1.215f, 0.00f, 1.0f));
            m_streak_dices[i][j].local_transform.position = new_position;
            m_streak_dices[i][j].local_transform.scale /= 7.5f;

            m_streak_dices[i][j].set_sprite_ptr(&m_dice_sprites.front());
            m_streak_dices[i][j].active = false;
        }
    }

    m_board_sprite.setup_sprite("assets/board.png");
    m_coin_red_sprite.setup_sprite("assets/coins/coin_red.png");
    m_board.set_sprite_ptr(&m_board_sprite);

    for(size_t i = 0; i < 4; ++i)
    {
        m_red_coins[i].set_sprite_ptr(&m_coin_red_sprite);
        m_red_coins[i].local_transform.position = m_board_handler.const_blocks()[0]
            .const_cells()[18 + i].const_position();
        m_red_coins[i].local_transform.position.z = 0.01f;
        m_red_coins[i].local_transform.scale /= 10.0f;
    }

    m_board.local_transform.scale = glm::vec3(2.0f, 2.0f, 1.0f);
    main_camera.transform.position.z = 3.5f;

    m_gameobject_ptrs.push_back(&m_board);

    for(size_t i = 0; i < 4; ++i)
    {
        m_gameobject_ptrs.push_back(&m_red_coins[i]);
    }

    m_dices[0].callbacks.push_back([this]() -> void
    {
        if(m_move)
        {
            return;
        }

        if(m_turn == 0)
        {
            m_dices[0].active = false;
            m_spinners[0].active = true;

            m_spinner_animations[0]->play();
        }
    });

    m_dices[2].callbacks.push_back([this]() -> void
    {
        if(m_move)
        {
            return;
        }

        if(m_turn == 2)
        {
            m_dices[2].active = false;
            m_spinners[2].active = true;

            m_spinner_animations[2]->play();
        }
    });

    for(size_t i = 0; i < 4; ++i)
    {
        m_ui_element_ptrs.push_back(&m_dices[i]);
        m_ui_element_ptrs.push_back(&m_spinners[i]);

        for(size_t j = 0; j < 3; ++j)
        {
            m_ui_element_ptrs.push_back(&m_streak_dices[i][j]);
        }
    }
}

void ludo::match_scene::on_update()
{
    const ludo::input::status &current_mouse_status =
        ludo::input::get_key(ludo::input::key::mouse_left);

    if(m_move)
    {
        glm::vec2 mouse_pos = ludo::input::get_mouse().const_position();
        mouse_pos.x = (mouse_pos.x * 2.0f) / ludo::screen::window_width() - 1.0f;
        mouse_pos.y = -(mouse_pos.y * 2.0f) / ludo::screen::window_height() + 1.0f;

        for(size_t i = 0; i < 4; ++i)
        {
            if(m_previous_mouse_status == ludo::input::status::press
                && current_mouse_status == ludo::input::status::release)
            {
                const glm::vec3 &position = m_red_coins[i].local_transform.position;
                const glm::quat &rotation = m_red_coins[i].local_transform.rotation;
                const glm::vec3 &scale = m_red_coins[i].local_transform.scale;
                glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), position);
                local_transform *= glm::toMat4(rotation);
                local_transform = glm::scale(local_transform, scale);
                const glm::vec3 &cam_position = main_camera.transform.position;
                const glm::quat &cam_rotation = main_camera.transform.rotation;
                const glm::mat4x4 &rotation_mat = glm::toMat4(cam_rotation);
                const glm::vec3 forward = rotation_mat * glm::vec4({0.0f, 0.0f, -1.0f, 1.0f});
                const glm::vec3 up = rotation_mat * glm::vec4({0.0f, 1.0f, 0.0f, 1.0f});
                const glm::mat4x4 view = glm::lookAt(cam_position, cam_position + forward, up);
                const glm::mat4x4 pxv = main_camera.projection * view;
                glm::vec4 position_vec4(position, 1.0f);
                position_vec4 = pxv * local_transform * position_vec4;
                position_vec4 /= position_vec4.w;
                const float distance = std::sqrt(std::pow(mouse_pos.x - position_vec4.x, 2.0f)
                    + std::pow(mouse_pos.y - position_vec4.y, 2.0f));

                if(distance <= 0.1f)
                {
                    m_move = false;
                    m_turn += m_player_count;
                    m_turn %= 4;

                    m_dices[m_turn].set_sprite_ptr(&m_act_dice_sprites[m_dice_values[m_turn] - 1]);

                    break;
                }   
            }
        }
    }

    m_previous_mouse_status = current_mouse_status;
}

void ludo::match_scene::on_late_update()
{

}