#ifndef SCENE_H
#define SCENE_H

#include <space.hpp>
#include <sprite.hpp>
#include <vector>

namespace ludo
{
    class camera
    {
    private:
        ludo::transform m_transform;
        glm::mat4x4 m_projection;

    public:
        camera();
        camera(const ludo::transform &_transform);
        ludo::transform &transform();
        const ludo::transform &const_transform() const;
        glm::mat4x4 &projection();
        const glm::mat4x4 &const_projection() const;
    };

    class gameobject
    {
    private:
        bool m_active;
        ludo::transform m_local_transform;
        ludo::sprite *m_sprite_ptr;
        std::vector<ludo::gameobject *> m_children_ptrs;

    public:
        gameobject();
        bool &active();
        const bool &const_active() const;
        ludo::transform &local_transform();
        const ludo::transform &const_local_transform() const;
        ludo::sprite *get_sprite_ptr() const;
        void set_sprite_ptr(ludo::sprite *_sprite_ptr);
        std::vector<ludo::gameobject *> &children_ptrs();
        const std::vector<ludo::gameobject *> &const_children_ptrs() const;
        void draw(const glm::mat4x4 &_global_transform) const;
    };

    class scene
    {
    protected:
        ludo::camera m_camera;
        glm::mat4x4 projection;
        std::vector<ludo::gameobject *> m_gameobject_ptrs;

    public:
        scene();
        ludo::camera &camera();
        const ludo::camera &const_camera() const;
        void draw() const;
        virtual void on_update() = 0;
        virtual void on_late_update() = 0;
        void simulate_physics() const;
    };
}

#endif