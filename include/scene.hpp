#ifndef SCENE_H
#define SCENE_H

#include <sprite.hpp>
#include <input.hpp>
#include <vector>
#include <functional>
#include <glm/ext.hpp>

namespace ludo
{
    class transform
    {
    public:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        transform();
        transform(const glm::vec3 &_position, const glm::quat &_rotation, const glm::vec3 &_scale);
    };

    class camera
    {
    public:
        ludo::transform transform;
        glm::mat4 projection;

        camera();
        camera(const ludo::transform &_transform);
    };

    class gameobject
    {
    private:
        ludo::sprite *m_sprite_ptr;

    public:
        bool active;
        ludo::transform local_transform;
        std::vector<ludo::gameobject *> children_ptrs;

        gameobject();
        ludo::sprite *get_sprite_ptr() const;
        void set_sprite_ptr(ludo::sprite *_sprite_ptr);
        void draw(const glm::mat4 &_global_transform) const;
        virtual ~gameobject();
    };

    class event_listener : public gameobject
    {
    public:
        std::vector<std::function<void()>> callbacks;

        event_listener();
        void listen(const glm::vec4 &_mouse_pos);
        void execute_callbacks();
    };

    class button : public event_listener
    {
    public:
        button();
    };

    class scene
    {
    private:
        glm::mat4 m_ui_projection;
        static ludo::input::status s_left_mouse_previous_status;

    protected:
        std::vector<ludo::gameobject *> m_gameobject_ptrs;
        std::vector<ludo::gameobject *> m_ui_element_ptrs;

    public:
        ludo::camera main_camera;

        scene();
        void draw() const;
        virtual void on_update() = 0;
        virtual void on_late_update() = 0;
        void listen_events();
        void simulate_physics() const;
        virtual ~scene();
    };
}

#endif