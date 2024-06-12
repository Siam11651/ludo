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
    loop(false) {}

void ludo::animation::play()
{
    if(keyframes.size() == 0)
    {
        return;
    }

    if(keyframes.front().transform_opt.has_value())
    {
        m_gameobject_ptr->local_transform = keyframes.front().transform_opt.value();
    }

    ludo::sprite *new_sprite = keyframes.front().get_sprite_ptr();

    if(new_sprite)
    {
        m_gameobject_ptr->set_sprite_ptr(new_sprite);
    }

    if(keyframes.size() == 1)
    {
        return;
    }

    m_current_keyframe_idx = 1;
    m_start_point = ludo::clk::now();

    keyframes.front().on_reach();

    m_playing = true;
    ludo::animation::s_current_animation_ptrs.insert(this);
}

void ludo::animation::stop()
{
    m_playing = false;
    s_current_animation_ptrs.erase(this);
}

const bool &ludo::animation::is_playing() const
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
        const std::vector<keyframe> &keyframes = animation_ptr->keyframes;

        if(animation_ptr->m_current_keyframe_idx >= animation_ptr->keyframes.size())
        {
            keyframes.back().on_reach();
            stop_queue.push(animation_ptr);

            continue;
        }

        const ludo::clk::time_point now = ludo::clk::now();
        size_t &current_idx = animation_ptr->m_current_keyframe_idx;
        const size_t prev_idx = animation_ptr->m_current_keyframe_idx - 1;
        const ludo::clk::duration &delay = keyframes[current_idx].delay;

        if(now >= animation_ptr->m_start_point + delay)
        {
            ++current_idx;
            animation_ptr->m_start_point = now;

            keyframes[prev_idx].on_reach();

            continue;
        }

        ludo::sprite *new_sprite = keyframes[current_idx].get_sprite_ptr();

        if(new_sprite)
        {
            animation_ptr->m_gameobject_ptr->set_sprite_ptr(new_sprite);
        }

        if(keyframes[current_idx].transform_opt.has_value())
        {
            if(keyframes[prev_idx].transform_opt.has_value())
            {
                const ludo::transform &start = animation_ptr->keyframes[prev_idx]
                    .transform_opt.value();
                const ludo::transform &end = animation_ptr->keyframes[current_idx]
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

        if(front->keyframes.back().transform_opt.has_value())
        {
            front->m_gameobject_ptr->local_transform = 
                front->keyframes.back().transform_opt.value();
        }

        if(front->loop)
        {
            front->play();
        }
        else
        {
            front->stop();
        }
    }
}