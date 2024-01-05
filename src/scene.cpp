#include <scene.hpp>
#include <screen.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

ludo::transform::transform() : 
    position(0.0f, 0.0f, 0.0f),
    rotation(1.0f, 0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f) {}

ludo::transform::transform(const glm::vec3 &_position, const glm::quat &_rotation, const glm::vec3 &_scale) :
    position(_position),
    rotation(_rotation),
    scale(_scale) {}

ludo::camera::camera() :
    transform(glm::vec3(0.0f, 0.0f, 1.0f), glm::quat(), glm::vec3(1.0f)),
    projection(glm::perspective(glm::radians(45.0f), screen::aspect_ratio(), 0.01f, 100.0f)) {}

ludo::camera::camera(const ludo::transform &_transform) :
    transform(_transform),
    projection(glm::perspective(90.0f, screen::aspect_ratio(), 0.01f, 100.0f)) {}

ludo::gameobject::gameobject() :
    active(true) {}

ludo::sprite *ludo::gameobject::get_sprite_ptr() const
{
    return m_sprite_ptr;
}

void ludo::gameobject::set_sprite_ptr(ludo::sprite *_sprite_ptr)
{
    m_sprite_ptr = _sprite_ptr;
}

void ludo::gameobject::draw(const glm::mat4x4 &_global_transform) const
{
    if(!active)
    {
        return;
    }

    if(m_sprite_ptr)
    {
        const glm::vec3 &position = local_transform.position;
        const glm::quat &rotation = local_transform.rotation;
        const glm::vec3 &scale = local_transform.scale;
        glm::mat4x4 local_transform = glm::translate(glm::identity<glm::mat4x4>(), position);
        local_transform *= glm::toMat4(rotation);
        local_transform = glm::scale(local_transform, scale);
        const glm::mat4x4 new_global_transform = _global_transform * local_transform;

        m_sprite_ptr->draw(new_global_transform);
    }
}

ludo::scene::scene() {}

void ludo::scene::draw() const
{
    const glm::vec3 &position = camera.transform.position;
    const glm::quat &rotation = camera.transform.rotation;
    const glm::mat4x4 &rotation_mat = glm::toMat4(rotation);
    const glm::vec3 forward = rotation_mat * glm::vec4({0.0f, 0.0f, -1.0f, 1.0f});
    const glm::vec3 up = rotation_mat * glm::vec4({0.0f, 1.0f, 0.0f, 1.0f});
    const glm::mat4x4 view = glm::lookAt(position, position + forward, up);
    const glm::mat4x4 pxv = camera.projection * view;
    
    for(ludo::gameobject *gameobject_ptr : m_gameobject_ptrs)
    {
        if(gameobject_ptr)
        {
            gameobject_ptr->draw(pxv);
        }
    }
}