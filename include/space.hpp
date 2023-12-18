#ifndef SPACE_H
#define SPACE_H

namespace ludo
{
    class vector2
    {
    private:
        float m_x;
        float m_y;

    public:
        static vector2 zero;
        static vector2 one;

        vector2(const float &_x, const float &_y);
        float &x();
        const float &const_x() const;
        float &y();
        const float &const_y() const;
    };

    class transform
    {
    private:
        vector2 m_position;
        float m_rotation;
        vector2 m_scale;

    public:
        transform();
        transform(const vector2 &_position, const float &_rotation, const vector2 &_scale);
        vector2 &position();
        const vector2 &const_position() const;
        float &rotation();
        const float &const_rotation() const;
        vector2 &scale();
        const vector2 &const_scale() const;
    };
}

#endif