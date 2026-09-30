#include "core/DInstance.hpp"
#include "core/DSurface.hpp"
#include "core/DWindow.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace dreamsengine;

int main()
{
    glfwInit();

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    DWindow* window = new DWindow();
    window->create_window(800, 600, "DreamsEngine");

    DInstance* instance = new DInstance();
    DSurface* surface = new DSurface(*instance, *window);

    while (!glfwWindowShouldClose(window->get_window())) {
        glfwSwapBuffers(window->get_window());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
