#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace ludo
{
    class transform
    {
    private:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

    public:
        transform();
        transform(const glm::vec3 &_position, const glm::quat &_rotation, const glm::vec3 &_scale);
        glm::vec3 &position();
        const glm::vec3 &const_position() const;
        glm::quat &rotation();
        const glm::quat &const_rotation() const;
        glm::vec3 &scale();
        const glm::vec3 &const_scale() const;
    };
}

#endif