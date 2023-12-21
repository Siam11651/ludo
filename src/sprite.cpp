#include <fstream>
#include <iostream>
#include <sstream>
#include <sprite.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image/stb_image.h>

GLuint ludo::sprite::s_vertex_buffer_object;
GLuint ludo::sprite::s_vertex_array_object;
GLuint ludo::sprite::s_element_buffer_object;
GLuint ludo::sprite::s_shader_program;
std::string vertshader_source =
R"(
#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;
out vec2 tex_coord;

void main()
{
    gl_Position = vec4(in_pos, 1.0f);
    tex_coord = in_tex_coord;
}
)";
std::string fragshader_source =
R"(
#version 460 core

in vec2 tex_coord;
out vec4 frag_color;
uniform sampler2D tex_sampler;

void main()
{
    frag_color = texture(tex_sampler, tex_coord);
}
)";

ludo::sprite::sprite()
{

}

ludo::sprite::sprite(const std::string &_filepath)
{
    setup_sprite(_filepath);
}

void ludo::sprite::setup_sprite(const std::string &_filepath)
{
    int32_t channel_count;
    m_data = stbi_load(_filepath.c_str(), &m_width, &m_height, &channel_count, 0);
    
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    if(channel_count == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

void ludo::sprite::draw(const glm::mat4x4 &_global_transform) const
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

ludo::sprite::~sprite()
{
    stbi_image_free(m_data);
}

void ludo::sprite::initialise()
{
    const char *vertshader_source_cstr = vertshader_source.c_str();
    const char *fragshader_source_cstr = fragshader_source.c_str();
    GLuint vertshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    s_shader_program = glCreateProgram();

    glShaderSource(vertshader, 1, &vertshader_source_cstr, nullptr);
    glShaderSource(fragshader, 1, &fragshader_source_cstr, nullptr);
    glCompileShader(vertshader);
    glCompileShader(fragshader);

    int32_t success;
    char info_log[1024];

    glGetShaderiv(vertshader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertshader, 1024, nullptr, info_log);

        std::cerr << info_log << std::endl;

        throw std::exception();
    }

    glGetShaderiv(fragshader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragshader, 1024, nullptr, info_log);

        std::cerr << info_log << std::endl;

        throw std::exception();
    }

    glAttachShader(s_shader_program, vertshader);
    glAttachShader(s_shader_program, fragshader);
    glLinkProgram(s_shader_program);
    glGetProgramiv(s_shader_program, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(s_shader_program, 1024, NULL, info_log);
        
        std::cerr << info_log << std::endl;

        throw std::exception();
    }

    glDeleteShader(vertshader);
    glDeleteShader(fragshader);
    glUseProgram(s_shader_program);

    const GLfloat vertices[] =
    {
         1.0f,  1.0f, 0.0f,     1.0f, 0.0f,   // top right
         1.0f, -1.0f, 0.0f,     1.0f, 1.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,     0.0f, 1.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,     0.0f, 0.0f    // top left 
    };
    const GLuint elements[] = {0, 3, 2, 0, 2, 1};

    glGenVertexArrays(1, &s_vertex_array_object);
    glGenBuffers(1, &s_vertex_buffer_object);
    glGenBuffers(1, &s_element_buffer_object);
    glBindVertexArray(s_vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void *)vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), (void *)elements, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}