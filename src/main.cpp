#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <screen.hpp>
#include <sprite.hpp>

int main()
{
    screen::window_width() = 720;
    screen::window_height() = 960;
    screen::window_title() = "Ludo";

    if(!glfwInit())
    {
        std::cerr << "Failed to initialise GLFW" << std::endl;

        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screen::window_width(), screen::window_height(),
        screen::window_title().c_str(), nullptr, nullptr);

    if(!window)
    {
        std::cerr << "Failed to create window" << std::endl;

        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialise GLAD" << std::endl;

        return -1;
    }

    sprite::initialise();

    sprite board("assets/board.png");

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        board.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}