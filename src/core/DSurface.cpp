#include "core/DSurface.hpp"
#include "core/DInstance.hpp"
#include "core/DWindow.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <stdexcept>

using namespace dreamsengine;

DSurface::DSurface(DInstance& instance, DWindow& window) : instance(instance), window(window)
{
    create_surface();
}

DSurface::~DSurface()
{
    vkDestroySurfaceKHR(instance.get_instance(), surface, NULL);
}

void DSurface::create_surface()
{
    VkResult result;
    result = glfwCreateWindowSurface(instance.get_instance(), window.get_window(), NULL, &surface);

    if (result != VK_SUCCESS) {
        throw std::runtime_error(FILE_PREFIX + "ERROR: Failed to create window surface.");
    }
}
