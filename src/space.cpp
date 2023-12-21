#include <space.hpp>

ludo::transform::transform() : 
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f) {}

ludo::transform::transform(const glm::vec3 &_position, const glm::quat &_rotation, const glm::vec3 &_scale) :
    m_position(_position),
    m_rotation(_rotation),
    m_scale(_scale) {}

glm::vec3 &ludo::transform::position()
{
    return m_position;
}

const glm::vec3 &ludo::transform::const_position() const
{
    return m_position;
}

glm::quat &ludo::transform::rotation()
{
    return m_rotation;
}

const glm::quat &ludo::transform::const_rotation() const
{
    return m_rotation;
}

glm::vec3 &ludo::transform::scale()
{
    return m_scale;
}

const glm::vec3 &ludo::transform::const_scale() const
{
    return m_scale;
}