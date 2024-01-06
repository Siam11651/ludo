#include <scene.hpp>
#include <screen.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

ludo::input::status ludo::scene::s_left_mouse_previous_status;

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

void ludo::gameobject::draw(const glm::mat4 &_global_transform) const
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
        glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), position);
        local_transform *= glm::toMat4(rotation);
        local_transform = glm::scale(local_transform, scale);
        const glm::mat4 new_global_transform = _global_transform * local_transform;

        m_sprite_ptr->draw(new_global_transform);
    }
}

ludo::gameobject::~gameobject()
{

}

ludo::event_listener::event_listener() {}

void ludo::event_listener::listen(const glm::vec4 &_mouse_pos)
{
    const glm::vec3 &position = local_transform.position;
    const glm::quat &rotation = local_transform.rotation;
    const glm::vec3 &scale = local_transform.scale;
    const glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), -position);
    const glm::mat4 local_rotation = glm::toMat4(-rotation);
    const glm::mat4 local_scale = glm::inverse(glm::scale(glm::mat4(1.0f), scale));
    glm::vec4 mouse_pos_vec4 = local_transform * _mouse_pos;
    mouse_pos_vec4 = local_rotation * mouse_pos_vec4;
    mouse_pos_vec4 = local_scale * mouse_pos_vec4;

    if(-0.5f <= mouse_pos_vec4.x && mouse_pos_vec4.x <= 0.5f
        && -0.5f <= mouse_pos_vec4.y && mouse_pos_vec4.y <= 0.5f)
    {
        execute_callbacks();
    }
}

void ludo::event_listener::execute_callbacks()
{
    for(const std::function<void()> &callback : callbacks)
    {
        callback();
    }
}

ludo::button::button() {}

ludo::scene::scene()
{
    const float aspect_ratio = ludo::screen::aspect_ratio();

    if(aspect_ratio >= 1.0f)
    {
        m_ui_projection = glm::scale(glm::identity<glm::mat4>(),
            glm::vec3(1.0f / aspect_ratio, 1.0f, 1.0f));
    }
    else
    {
        m_ui_projection = glm::scale(glm::identity<glm::mat4>(),
            glm::vec3(1.0f, 1.0f * aspect_ratio, 1.0f));
    }
}

void ludo::scene::draw() const
{
    const glm::vec3 &position = main_camera.transform.position;
    const glm::quat &rotation = main_camera.transform.rotation;
    const glm::mat4x4 &rotation_mat = glm::toMat4(rotation);
    const glm::vec3 forward = rotation_mat * glm::vec4({0.0f, 0.0f, -1.0f, 1.0f});
    const glm::vec3 up = rotation_mat * glm::vec4({0.0f, 1.0f, 0.0f, 1.0f});
    const glm::mat4x4 view = glm::lookAt(position, position + forward, up);
    const glm::mat4x4 pxv = main_camera.projection * view;
    
    for(ludo::gameobject *gameobject_ptr : m_gameobject_ptrs)
    {
        if(gameobject_ptr)
        {
            gameobject_ptr->draw(pxv);
        }
    }

    for(ludo::gameobject *ui_element_ptr : m_ui_element_ptrs)
    {
        if(ui_element_ptr)
        {
            ui_element_ptr->draw(m_ui_projection);
        }
    }
}

void ludo::scene::listen_events()
{
    const ludo::input::status current_status = ludo::input::get_key(ludo::input::key::mouse_left);

    if(s_left_mouse_previous_status == ludo::input::status::press
        && current_status == ludo::input::status::release)
    {
        const glm::vec2 &mouse_pos = ludo::input::get_mouse().const_position();
        glm::vec4 mouse_pos_vec4(0.0f);
        mouse_pos_vec4.x = (mouse_pos.x * 2.0f) / ludo::screen::window_width - 1.0f;
        mouse_pos_vec4.y = -(mouse_pos.y * 2.0f) / ludo::screen::window_height + 1.0f;
        mouse_pos_vec4.w = 1.0f;
        mouse_pos_vec4 = glm::inverse(m_ui_projection) * mouse_pos_vec4;

        for(ludo::gameobject *ui_element_ptr : m_ui_element_ptrs)
        {
            ludo::event_listener *event_listener_ptr = dynamic_cast<ludo::event_listener *>(ui_element_ptr);

            if(event_listener_ptr && event_listener_ptr->active)
            {
                event_listener_ptr->listen(mouse_pos_vec4);
            }
        }   
    }

    s_left_mouse_previous_status = current_status;
}

ludo::scene::~scene()
{

}