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

void ludo::game_object::draw(glm::mat4x4 &_global_transform) const
{
    if(m_sprite_ptr)
    {
        const glm::vec2 &position = m_local_transform.const_position();
        const float &rotation = m_local_transform.const_rotation();
        const glm::vec2 &scale = m_local_transform.const_scale();
        glm::mat4x4 translation_mat(1.0f);
        translation_mat[0][3] = position.x;
        translation_mat[1][3] = position.y;
        glm::mat4x4 rotation_mat(1.0f);
        rotation_mat[0][0] = std::cos(rotation);
        rotation_mat[0][1] = -std::sin(rotation);
        rotation_mat[1][0] = std::sin(rotation);
        rotation_mat[1][1] = -std::cos(rotation);
        glm::mat4x4 scale_mat(1.0f);
        scale_mat[0][0] = scale.x;
        scale_mat[1][1] = scale.y;
        glm::mat4x4 new_global_transform = _global_transform * translation_mat * rotation_mat * scale_mat;

        m_sprite_ptr->draw(new_global_transform);
    }
}