#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <string>
#include <cstdint>
#include <glad/glad.h>

namespace ludo
{
    class sprite
    {
    private:
        int32_t m_width;
        int32_t m_height;
        uint8_t *m_data;
        GLuint m_texture;
        static std::string vertshader_source_str;
        static std::string fragshader_source_str;
        static GLuint s_vertex_buffer_object;
        static GLuint s_vertex_array_object;
        static GLuint s_element_buffer_object;
        static GLuint s_shader_program;

    public:
        sprite();
        sprite(const std::string &_filepath);
        void setup_sprite(const std::string &_filepath);
        void draw(const glm::mat4x4 &_global_transform) const;
        ~sprite();
        static void initialise();
    };
}

#endif