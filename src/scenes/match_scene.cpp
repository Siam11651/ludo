#include <sstream>
#include <iostream>
#include <scenes/match_scene.hpp>
#include <animation.hpp>
#include <input.hpp>

ludo::match_scene::match_scene() :
    scene()
{
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
                    this->m_dices[k].active = true;
                    this->m_spinners[k].active = false;
                    this->m_dice_values[k] = this->rand_gen() % 6 + 1;
                    this->m_streak_dices[k][this->m_moves[k].size()].active = true;

                    if(this->m_moves[k].size() > 0 && this->m_moves[k].back() == 6)
                    {
                        this->m_moves[k].push_back(this->m_dice_values[k]);
                    }
                    else
                    {
                        this->m_moves[k] = {this->m_dice_values[k]};
                    }

                    for(size_t j = 0; j < 3; ++j)
                    {
                        if(j < this->m_moves[k].size())
                        {
                            this->m_streak_dices[k][j].active = true;

                            this->m_streak_dices[k][j].set_sprite_ptr(
                                &this->m_act_dice_sprites[this->m_moves[k][j] - 1]);
                        }
                        else
                        {
                            this->m_streak_dices[k][j].active = false;
                        }
                    }

                    if(this->m_moves[k].size() == 3)
                    {
                        if(this->m_moves[k].back() < 6)
                        {
                            // let make move
                        }
                        else
                        {
                            this->m_turn += this->m_player_count;
                            this->m_turn %= 4;
                        }

                        this->m_moves[k].clear();
                    }
                    else if(this->m_moves[k].back() < 6)
                    {
                        // make move

                        this->m_turn += this->m_player_count;
                        this->m_turn %= 4;
                    }

                    this->m_dices[this->m_turn].set_sprite_ptr(&this->m_act_dice_sprites[this
                            ->m_dice_values[this->m_turn] - 1]);

                    if(k != this->m_turn)
                    {
                        this->m_dices[k].set_sprite_ptr(&this->m_dice_sprites[this
                            ->m_dice_values[k] - 1]);
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

    m_curren_cell_ptr = &m_board_handler.const_blocks().front().const_cells()[7];

    for(size_t i = 0; i < 4; ++i)
    {
        const glm::mat4x4 rotation_mat = glm::rotate(glm::identity<glm::mat4x4>(),
            (float)(M_PI * i) / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 new_position = glm::vec3(rotation_mat
            * glm::vec4(-0.89f, -1.11f, 0.01f, 1.0f));
        m_spinners[i].active = false;
        m_spinners[i].local_transform.position = new_position;
        m_spinners[i].local_transform.scale /= 5.0f;
        new_position = glm::vec3(rotation_mat
            * glm::vec4(-0.89f, -1.11f, 0.01f, 1.0f));
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
                * glm::vec4(-0.9f + j * 0.15f, -1.3f, 0.01f, 1.0f));
            m_streak_dices[i][j].local_transform.position = new_position;
            m_streak_dices[i][j].local_transform.scale /= 7.5f;

            m_streak_dices[i][j].set_sprite_ptr(&m_dice_sprites.front());
            m_streak_dices[i][j].active = false;
        }
    }

    m_button_lefts = {40.0f, 628.0f, 628.0f, 40.0f};
    m_button_rights = {92.0f, 680.0f, 680.0f, 92.0f};
    m_button_tops = {825.0f, 825.0f, 83.0f, 83.0f};
    m_button_bots = {877.0f, 877.0f, 135.0f, 135.0f};

    m_board_sprite.setup_sprite("assets/board.png");
    m_coin_red_sprite.setup_sprite("assets/coins/coin_red.png");
    m_board.set_sprite_ptr(&m_board_sprite);
    m_coin_red.set_sprite_ptr(&m_coin_red_sprite);
    m_board.local_transform.scale = glm::vec3(2.0f, 2.0f, 1.0f);
    m_coin_red.local_transform.position = m_curren_cell_ptr->const_position();
    m_coin_red.local_transform.position.z = 0.01f;
    m_coin_red.local_transform.scale /= 10.0f;
    camera.transform.position.z = 3.5f;

    m_gameobject_ptrs.insert(m_gameobject_ptrs.end(),
    {
        &m_board,
        &m_coin_red
    });

    for(size_t i = 0; i < 4; ++i)
    {
        m_gameobject_ptrs.push_back(&m_dices[i]);
        m_gameobject_ptrs.push_back(&m_spinners[i]);

        for(size_t j = 0; j < 3; ++j)
        {
            m_gameobject_ptrs.push_back(&m_streak_dices[i][j]);
        }
    }
}

void ludo::match_scene::on_update()
{
    const ludo::input::status current_status = ludo::input::get_key(ludo::input::key::mouse_left);

    if(m_previous_status == ludo::input::status::press
        && current_status == ludo::input::status::release)
    {
        const glm::vec2 &mouse_pos = ludo::input::get_mouse().const_position();

        for(size_t i = 0; i < 4; ++i)
        {
            if(m_turn != i)
            {
                continue;
            }

            if(m_button_lefts[i] <= mouse_pos.x && mouse_pos.x <= m_button_rights[i]
                && m_button_tops[i] <= mouse_pos.y && mouse_pos.y <= m_button_bots[i])
            {
                m_dices[i].active = false;
                m_spinners[i].active = true;

                m_spinner_animations[i]->play();

                break;
            }
        }
    }

    m_previous_status = current_status;
}

void ludo::match_scene::on_late_update()
{

}