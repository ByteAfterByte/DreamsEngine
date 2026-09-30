#include "core/DWindow.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

using namespace dreamsengine;

DWindow::DWindow()
{
}

DWindow::~DWindow()
{
    glfwDestroyWindow(window);
}

void DWindow::create_window(unsigned int width, unsigned int height, const char* title)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    this->window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
        throw std::runtime_error(FILE_PREFIX + "ERROR: Failed to create window.");
    }

    glfwMakeContextCurrent(window);
}
