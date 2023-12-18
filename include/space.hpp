#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>

namespace ludo
{
    class transform
    {
    private:
        glm::vec2 m_position;
        float m_rotation;
        glm::vec2 m_scale;

    public:
        transform();
        transform(const glm::vec2 &_position, const float &_rotation, const glm::vec2 &_scale);
        glm::vec2 &position();
        const glm::vec2 &const_position() const;
        float &rotation();
        const float &const_rotation() const;
        glm::vec2 &scale();
        const glm::vec2 &const_scale() const;
    };
}

#endif