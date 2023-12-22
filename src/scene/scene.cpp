#include <scene/scene.hpp>
#include <screen.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

ludo::camera::camera() :
    m_transform(glm::vec3(0.0f, 0.0f, 1.0f), glm::quat(), glm::vec3(1.0f)),
    m_projection(glm::perspective(glm::radians(45.0f), screen::aspect_ratio(), 0.01f, 100.0f)) {}

ludo::camera::camera(const ludo::transform &_transform) :
    m_transform(_transform),
    m_projection(glm::perspective(90.0f, screen::aspect_ratio(), 0.01f, 100.0f)) {}

ludo::transform &ludo::camera::transform()
{
    return m_transform;
}

const ludo::transform &ludo::camera::const_transform() const
{
    return m_transform;
}

glm::mat4x4 &ludo::camera::projection()
{
    return m_projection;
}

const glm::mat4x4 &ludo::camera::const_projection() const
{
    return m_projection;
}

ludo::gameobject::gameobject() {}

ludo::transform &ludo::gameobject::local_transform()
{
    return m_local_transform;
}

const ludo::transform &ludo::gameobject::const_local_transform() const
{
    return m_local_transform;
}

ludo::sprite *ludo::gameobject::get_sprite_ptr() const
{
    return m_sprite_ptr;
}

void ludo::gameobject::set_sprite_ptr(ludo::sprite *_sprite_ptr)
{
    m_sprite_ptr = _sprite_ptr;
}

std::vector<ludo::gameobject *> &ludo::gameobject::children_ptrs()
{
    return m_children_ptrs;
}

const std::vector<ludo::gameobject *> &ludo::gameobject::const_children_ptrs() const
{
    return m_children_ptrs;
}

void ludo::gameobject::draw(const glm::mat4x4 &_global_transform) const
{
    if(m_sprite_ptr)
    {
        const glm::vec3 &position = m_local_transform.const_position();
        const glm::quat &rotation = m_local_transform.const_rotation();
        const glm::vec3 &scale = m_local_transform.const_scale();
        glm::mat4x4 local_transform = glm::translate(glm::identity<glm::mat4x4>(), position);
        local_transform *= glm::toMat4(rotation);
        local_transform = glm::scale(local_transform, scale);
        const glm::mat4x4 new_global_transform = _global_transform * local_transform;

        m_sprite_ptr->draw(new_global_transform);
    }
}

ludo::scene::scene() {}

ludo::camera &ludo::scene::camera()
{
    return m_camera;
}

const ludo::camera &ludo::scene::const_camera() const
{
    return m_camera;
}

void ludo::scene::draw() const
{
    const glm::vec3 &position = m_camera.const_transform().const_position();
    const glm::quat &rotation = m_camera.const_transform().const_rotation();
    const glm::mat4x4 &rotation_mat = glm::toMat4(rotation);
    const glm::vec3 forward = rotation_mat * glm::vec4({0.0f, 0.0f, -1.0f, 1.0f});
    const glm::vec3 up = rotation_mat * glm::vec4({0.0f, 1.0f, 0.0f, 1.0f});
    const glm::mat4x4 view = glm::lookAt(position, position + forward, up);
    const glm::mat4x4 pxv = m_camera.const_projection() * view;
    
    for(ludo::gameobject *gameobject_ptr : m_gameobject_ptrs)
    {
        if(gameobject_ptr)
        {
            gameobject_ptr->draw(pxv);
        }
    }
}