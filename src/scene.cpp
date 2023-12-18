#include <scene.hpp>

ludo::camera::camera() {}

ludo::camera::camera(const ludo::transform &_transform) :
    m_transform(_transform) {}

ludo::transform &ludo::camera::transform()
{
    return m_transform;
}

const ludo::transform &ludo::camera::const_transform() const
{
    return m_transform;
}

ludo::game_object::game_object() {}

ludo::transform &ludo::game_object::local_transform()
{
    return m_local_transform;
}

const ludo::transform &ludo::game_object::const_local_transform() const
{
    return m_local_transform;
}

ludo::sprite *ludo::game_object::get_sprite() const
{
    return m_sprite_ptr;
}

void ludo::game_object::set_sprite(ludo::sprite *_sprite_ptr)
{
    m_sprite_ptr = _sprite_ptr;
}

std::vector<ludo::game_object *> &ludo::game_object::children_ptrs()
{
    return m_children_ptrs;
}

const std::vector<ludo::game_object *> &ludo::game_object::const_children_ptrs() const
{
    return m_children_ptrs;
}