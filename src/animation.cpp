#include <animation.hpp>
#include <scene/scene.hpp>
#include <queue>
#include <iostream>

std::set<ludo::animation *> ludo::animation::s_current_animation_ptrs;

ludo::keyframe::keyframe() :
    m_delay(0) {}

ludo::keyframe::keyframe(const ludo::clk::duration &_delay) :
    m_delay(_delay) {}

ludo::clk::duration &ludo::keyframe::delay()
{
    return m_delay;
}

const ludo::clk::duration &ludo::keyframe::const_delay() const
{
    return m_delay;
}

std::optional<ludo::transform> &ludo::keyframe::transform_opt()
{
    return m_transform_opt;
}

const std::optional<ludo::transform> &ludo::keyframe::const_transform_opt() const
{
    return m_transform_opt;
}

ludo::animation::animation(ludo::gameobject *_gameobject_ptr) :
    m_gameobject_ptr(_gameobject_ptr) {}

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

    if(!m_keyframes.front().transform_opt().has_value())
    {
        return;
    }

    m_gameobject_ptr->local_transform() = m_keyframes.front().transform_opt().value();

    if(m_keyframes.size() == 1)
    {
        return;
    }

    m_current_keyframe_idx = 0;
    m_start_point = ludo::clk::now();
    m_is_playing = true;
    ludo::animation::s_current_animation_ptrs.insert(this);
}

void ludo::animation::stop()
{
    m_is_playing = false;
    s_current_animation_ptrs.erase(this);
}

const bool &ludo::animation::get_is_playing() const
{
    return m_is_playing;
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
        const ludo::clk::duration delay = keyframes[current_idx].const_delay();

        if(now >= animation_ptr->m_start_point + delay)
        {
            animation_ptr->m_current_keyframe_idx = next_idx;
            animation_ptr->m_start_point = now;

            continue;
        }

        if(keyframes[current_idx].const_transform_opt().has_value())
        {
            if(keyframes[next_idx].const_transform_opt().has_value())
            {
                const ludo::transform &start = animation_ptr->m_keyframes[current_idx]
                    .const_transform_opt().value();
                const ludo::transform &end = animation_ptr->m_keyframes[next_idx]
                    .const_transform_opt().value();
                const glm::vec3 m = (end.const_position() - start.const_position()) / (float)delay.count();
                const ludo::clk::duration elapsed = now - animation_ptr->m_start_point;
                animation_ptr->m_gameobject_ptr->local_transform().position() = start.const_position()
                    + m * ((float)elapsed.count());
            }
            else
            {
                animation_ptr->m_gameobject_ptr->local_transform() =
                    keyframes[current_idx].const_transform_opt().value();
            }
        }
    }

    while(!stop_queue.empty())
    {
        ludo::animation *front = stop_queue.front();

        stop_queue.pop();

        if(front->m_keyframes.back().const_transform_opt().has_value())
        {
            front->m_gameobject_ptr->local_transform() = 
                front->m_keyframes.back().const_transform_opt().value();
        }

        s_current_animation_ptrs.erase(front);
    }
}