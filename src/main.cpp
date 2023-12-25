#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <screen.hpp>
#include <time.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <animation.hpp>
#include <scene/test_scene.hpp>
#include <scene/match_scene.hpp>

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ludo::sprite::initialise();

    ludo::scene *current_scene = new ludo::match_scene();
    // ludo::scene *current_scene = new ludo::test_scene();
    double mouse_pos_x;
    double mouse_pos_y;

    while(!glfwWindowShouldClose(window))
    {
        ludo::time::start_frame();
        glfwPollEvents();
        glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);

        int32_t mouse_left_status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        ludo::input::mouse new_mouse;
        new_mouse.position() = glm::vec2(mouse_pos_x, mouse_pos_y);

        ludo::input::set_key(ludo::input::key::mouse_left, (ludo::input::status)mouse_left_status);
        ludo::input::set_mouse(new_mouse);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        current_scene->on_update();
        ludo::animation::animate();
        current_scene->draw();
        current_scene->on_late_update();
        glfwSwapBuffers(window);
        ludo::time::end_frame();
    }

    return 0;
}