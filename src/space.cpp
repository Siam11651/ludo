#include <space.hpp>

ludo::vector2 ludo::vector2::zero(0.0f, 0.0f);
ludo::vector2 ludo::vector2::one(1.0f, 1.0f);

ludo::vector2::vector2(const float &_x, const float &_y) :
    m_x(_x),
    m_y(_y) {}

float &ludo::vector2::x()
{
    return m_x;
}

const float &ludo::vector2::const_x() const
{
    return m_x;
}

float &ludo::vector2::y()
{
    return m_y;
}

const float &ludo::vector2::const_y() const
{
    return m_y;
}

ludo::transform::transform() : 
    m_position(0.0f, 0.0f),
    m_scale(1.0f, 1.0f) {}

ludo::transform::transform(const vector2 &_position, const float &_rotation, const vector2 &_scale) :
    m_position(_position),
    m_rotation(_rotation),
    m_scale(_scale) {}

ludo::vector2 &ludo::transform::position()
{
    return m_position;
}

const ludo::vector2 &ludo::transform::const_position() const
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

ludo::vector2 &ludo::transform::scale()
{
    return m_scale;
}

const ludo::vector2 &ludo::transform::const_scale() const
{
    return m_scale;
}