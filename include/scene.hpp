#ifndef SCENE_H
#define SCENE_H

#include <sprite.hpp>
#include <vector>
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
        glm::mat4x4 projection;

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