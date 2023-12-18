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

    public:
        camera();
        camera(const ludo::transform &_transform);
        ludo::transform &transform();
        const ludo::transform &const_transform() const;
    };

    class game_object
    {
    private:
        ludo::transform m_local_transform;
        ludo::sprite *m_sprite_ptr;
        std::vector<game_object *> m_children_ptrs;

    public:
        game_object();
        ludo::transform &local_transform();
        const ludo::transform &const_local_transform() const;
        ludo::sprite *get_sprite() const;
        void set_sprite(ludo::sprite *_sprite_ptr);
        std::vector<game_object *> &children_ptrs();
        const std::vector<game_object *> &const_children_ptrs() const;
    };

    class scene
    {
    private:
        camera m_camera;
    public:

    };
}

#endif