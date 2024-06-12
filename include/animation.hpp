#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <vector>
#include <set>
#include <optional>
#include <functional>
#include <sprite.hpp>
#include <time.hpp>
#include <scene.hpp>

namespace ludo
{
    class gameobject;
    class animation;

    class keyframe
    {
    private:
        ludo::sprite *m_sprite_ptr;

    public:
        /**
         * @brief keyframe duration in nanoseconds
         * 
         */
        ludo::clk::duration delay;
        std::function<void()> on_reach;
        std::optional<ludo::transform> transform_opt;

        keyframe();
        keyframe(const ludo::clk::duration &_delay);
        void set_sprite_ptr(ludo::sprite *_sprite_ptr);
        ludo::sprite *get_sprite_ptr() const;
    };

    class animation
    {
    private:
        bool m_playing;
        size_t m_current_keyframe_idx;
        ludo::clk::time_point m_start_point;
        ludo::gameobject *m_gameobject_ptr;
        static std::set<animation *> s_current_animation_ptrs;

    public:
        bool loop;
        std::vector<ludo::keyframe> keyframes;

        animation(ludo::gameobject *_gameobject_ptr);
        const bool &is_playing() const;
        ludo::gameobject *get_gameobject_ptr() const;
        void play();
        void stop();
        ~animation();
        static void animate();
    };
}

#endif