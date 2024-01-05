#include <animation.hpp>
#include <scene.hpp>
#include <queue>
#include <iostream>

std::set<ludo::animation *> ludo::animation::s_current_animation_ptrs;

ludo::keyframe::keyframe() :
    delay(0),
    on_reach([]() -> void {}),
    m_sprite_ptr(nullptr) {}

ludo::keyframe::keyframe(const ludo::clk::duration &_delay) :
    delay(_delay),
    on_reach([]() -> void {}),
    m_sprite_ptr(nullptr) {}

void ludo::keyframe::set_sprite_ptr(ludo::sprite *_sprite_ptr)
{
    m_sprite_ptr = _sprite_ptr;
}

ludo::sprite *ludo::keyframe::get_sprite_ptr() const
{
    return m_sprite_ptr;
}

ludo::animation::animation(ludo::gameobject *_gameobject_ptr) :
    m_gameobject_ptr(_gameobject_ptr),
    m_playing(false),
    m_loop(false) {}

bool &ludo::animation::loop()
{
    return m_loop;
}

const bool &ludo::animation::const_loop() const
{
    return m_loop;
}

std::vector<ludo::keyframe> &ludo::animation::keyframes()
{
    return m_keyframes;
}

const std::vector<ludo::keyframe> &ludo::animation::const_keyframes() const
{
    return m_keyframes;
}

void ludo::animation::play()
{
    if(m_keyframes.size() == 0)
    {
        return;
    }

    if(m_keyframes.front().transform_opt.has_value())
    {
        m_gameobject_ptr->local_transform = m_keyframes.front().transform_opt.value();
    }

    ludo::sprite *new_sprite = m_keyframes.front().get_sprite_ptr();

    if(new_sprite)
    {
        m_gameobject_ptr->set_sprite_ptr(new_sprite);
    }

    if(m_keyframes.size() == 1)
    {
        return;
    }

    m_current_keyframe_idx = 0;
    m_start_point = ludo::clk::now();

    m_keyframes[m_current_keyframe_idx].on_reach();

    m_playing = true;
    ludo::animation::s_current_animation_ptrs.insert(this);
}

void ludo::animation::stop()
{
    m_playing = false;
    s_current_animation_ptrs.erase(this);
}

const bool &ludo::animation::get_playing() const
{
    return m_playing;
}

ludo::animation::~animation()
{
    s_current_animation_ptrs.erase(this);
}

void ludo::animation::animate()
{
    std::queue<animation *> stop_queue;

    for(ludo::animation *animation_ptr : s_current_animation_ptrs)
    {
        const std::vector<keyframe> &keyframes = animation_ptr->m_keyframes;

        if(animation_ptr->m_current_keyframe_idx >= animation_ptr->m_keyframes.size())
        {
            stop_queue.push(animation_ptr);

            continue;
        }

        const ludo::clk::time_point now = ludo::clk::now();
        const size_t current_idx = animation_ptr->m_current_keyframe_idx;
        const size_t next_idx = animation_ptr->m_current_keyframe_idx + 1;
        const ludo::clk::duration delay = keyframes[current_idx].delay;

        if(now >= animation_ptr->m_start_point + delay)
        {
            animation_ptr->m_current_keyframe_idx = next_idx;
            animation_ptr->m_start_point = now;

            if(next_idx < keyframes.size())
            {
                keyframes[next_idx].on_reach();
            }

            continue;
        }

        ludo::sprite *new_sprite = keyframes[current_idx].get_sprite_ptr();

        if(new_sprite)
        {
            animation_ptr->m_gameobject_ptr->set_sprite_ptr(new_sprite);
        }

        if(keyframes[current_idx].transform_opt.has_value())
        {
            if(keyframes[next_idx].transform_opt.has_value())
            {
                const ludo::transform &start = animation_ptr->m_keyframes[current_idx]
                    .transform_opt.value();
                const ludo::transform &end = animation_ptr->m_keyframes[next_idx]
                    .transform_opt.value();
                const glm::vec3 m = (end.position - start.position) / (float)delay.count();
                const ludo::clk::duration elapsed = now - animation_ptr->m_start_point;
                animation_ptr->m_gameobject_ptr->local_transform.position = start.position
                    + m * ((float)elapsed.count());
            }
            else
            {
                animation_ptr->m_gameobject_ptr->local_transform =
                    keyframes[current_idx].transform_opt.value();
            }
        }
    }

    while(!stop_queue.empty())
    {
        ludo::animation *front = stop_queue.front();

        stop_queue.pop();

        if(front->m_keyframes.back().transform_opt.has_value())
        {
            front->m_gameobject_ptr->local_transform = 
                front->m_keyframes.back().transform_opt.value();
        }

        if(front->m_loop)
        {
            front->play();
        }
        else
        {
            front->stop();
        }
    }
}