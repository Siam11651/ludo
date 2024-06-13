#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <screen.hpp>
#include <time.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <animation.hpp>
#include <scenes/match_scene.hpp>
#include <scenes/test_scene.hpp>

int main()
{
    ludo::screen::window_width = 720;
    ludo::screen::window_height = 960;
    ludo::screen::window_title = "Ludo";

    if(!glfwInit())
    {
        std::cerr << "Failed to initialise GLFW" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(ludo::screen::window_width,
        ludo::screen::window_height, ludo::screen::window_title.c_str(),
        nullptr, nullptr);

    if(!window)
    {
        std::cerr << "Failed to create window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialise GLAD" << std::endl;

        glfwTerminate();

        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    ludo::sprite::initialise();

    ludo::scene_manager::set_current_scene(new ludo::match_scene());
    // ludo::scene_manager::set_current_scene(new ludo::test_scene());
    double mouse_pos_x;
    double mouse_pos_y;

    while(!glfwWindowShouldClose(window))
    {
        ludo::time::start_frame();
        ludo::scene_manager::cleanup_previous_scene();
        glfwPollEvents();
        glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);

        ludo::scene *current_scene = ludo::scene_manager::get_current_scene();

        if(current_scene)
        {
            int32_t mouse_left_status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            ludo::input::mouse new_mouse;
            new_mouse.position = glm::vec2(mouse_pos_x, mouse_pos_y);

            ludo::input::set_key(ludo::input::key::mouse_left, (ludo::input::status)mouse_left_status);
            ludo::input::set_mouse(new_mouse);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            current_scene->on_update();
            ludo::animation::animate();
            current_scene->draw();
            current_scene->on_late_update();
            current_scene->listen_events();
        }

        glfwSwapBuffers(window);
        ludo::time::end_frame();
    }

    ludo::scene_manager::set_current_scene(nullptr);
    ludo::scene_manager::cleanup_previous_scene();
    glfwTerminate();

    return 0;
}