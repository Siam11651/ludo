#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <vector>
#include <set>
#include <optional>
#include <memory>
#include <time.hpp>
#include <space.hpp>

namespace ludo
{
    class gameobject;
    class animation;

    class keyframe
    {
    private:
        ludo::clk::duration m_delay;
        std::optional<ludo::transform> m_transform_opt;

    public:
        keyframe();
        keyframe(const ludo::clk::duration &_delay);
        ludo::clk::duration &delay();
        const ludo::clk::duration &const_delay() const;
        std::optional<ludo::transform> &transform_opt();
        const std::optional<ludo::transform> &const_transform_opt() const;
    };

    class animation
    {
    private:
        bool m_is_playing;
        size_t m_current_keyframe_idx;
        ludo::clk::time_point m_start_point;
        ludo::gameobject *m_gameobject_ptr;
        std::vector<ludo::keyframe> m_keyframes;
        static std::set<animation *> s_current_animation_ptrs;

    public:
        animation(ludo::gameobject *_gameobject_ptr);
        std::vector<ludo::keyframe> &keyframes();
        const std::vector<ludo::keyframe> &const_keyframes() const;
        const bool &get_is_playing() const;
        ludo::gameobject *get_gameobject_ptr() const;
        void play();
        void stop();
        ~animation();
        static void animate();
    };
}

#endif