#include <space.hpp>

ludo::transform::transform() : 
    m_position(0.0f, 0.0f),
    m_scale(1.0f, 1.0f) {}

ludo::transform::transform(const glm::vec2 &_position, const float &_rotation, const glm::vec2 &_scale) :
    m_position(_position),
    m_rotation(_rotation),
    m_scale(_scale) {}

glm::vec2 &ludo::transform::position()
{
    return m_position;
}

const glm::vec2 &ludo::transform::const_position() const
{
    return m_position;
}

float &ludo::transform::rotation()
{
    return m_rotation;
}

const float &ludo::transform::const_rotation() const
{
    return m_rotation;
}

glm::vec2 &ludo::transform::scale()
{
    return m_scale;
}

const glm::vec2 &ludo::transform::const_scale() const
{
    return m_scale;
}