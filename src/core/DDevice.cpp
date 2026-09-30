#include "core/DDevice.hpp"
#include "core/DInstance.hpp"
#include "core/DSurface.hpp"

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <vector>

using namespace dreamsengine;

DDevice::DDevice(DInstance& instance, DSurface& surface) : instance(instance), surface(surface)
{

}

DDevice::~DDevice()
{

}

bool DDevice::is_physical_device_suitable(VkPhysicalDevice physical_device)
{
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceFeatures physical_device_features;

    vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

    return physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physical_device_features.fillModeNonSolid && physical_device_features.geometryShader;
}

void DDevice::select_physical_device()
{
    uint32_t physical_devices_count;
    std::vector<VkPhysicalDevice> physical_devices;

    vkEnumeratePhysicalDevices(instance.get_instance(), &physical_devices_count, physical_devices.data());

    if (physical_devices_count == 0) {
        throw std::runtime_error(FILE_PREFIX + "ERROR: No GPUs found.");
    }

    VkPhysicalDevice physical_device;
    for (auto& device : physical_devices) {
        if (is_physical_device_suitable(physical_device)) {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error(FILE_PREFIX + "ERROR: Unable to find a suitable GPU.");
    }

    uint32_t queue_properties_count;
    std::vector<VkQueueFamilyProperties2> queue_family_properties;
    vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &queue_properties_count, queue_family_properties.data());

    uint32_t queue_family_index;
    for (auto properties : queue_family_properties) {
        VkBool32 present_support;
        VkResult result;
        result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface.get_surface(), &present_support);

        if (result != VK_SUCCESS) {
            throw std::runtime_error(FILE_PREFIX + "ERROR: Failed to get surface support for GPU.");
        }

        bool graphics_support = properties.queueFamilyProperties.queueFlags == VK_QUEUE_GRAPHICS_BIT;

        if (present_support && graphics_support) {
            graphics_queue_family = queue_family_index;
        }
    }
}

void DDevice::create_logical_device()
{

}
