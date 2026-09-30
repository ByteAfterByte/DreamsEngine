#include "core/DInstance.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <print>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using namespace dreamsengine;

DInstance::DInstance()
{
    if (!glfwVulkanSupported())
        throw std::runtime_error(FILE_PREFIX + "Vulkan isn't supported, unable to continue.");

    glfwInitVulkanLoader(vkGetInstanceProcAddr);
    create_instance();
}

DInstance::~DInstance()
{
    vkDestroyInstance(instance, NULL);
}

void DInstance::create_instance()
{
    VkApplicationInfo application_info = VkApplicationInfo{
        .pEngineName = "DreamsEngine",
        .engineVersion = VK_MAKE_API_VERSION(0, 26, 0, 0),
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0),
    };

    uint32_t extension_count;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    VkInstanceCreateInfo instance_create_info = VkInstanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &application_info,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions,
    };

    VkResult result = vkCreateInstance(&instance_create_info, NULL, &instance);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(FILE_PREFIX + "ERROR: Failed to create the Vulkan Instance.");
        return;
    }
}
