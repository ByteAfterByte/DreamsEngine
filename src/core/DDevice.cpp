#include "core/DDevice.hpp"
#include "core/DInstance.hpp"
#include "core/DSurface.hpp"

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <vector>
#include <set>

using namespace dreamsengine;

DDevice::DDevice(DInstance &instance, DSurface &surface) : instance(instance), surface(surface)
{
    select_physical_device();
    create_logical_device();
}

DDevice::~DDevice()
{
    if (logical_device != VK_NULL_HANDLE)
        vkDestroyDevice(logical_device, nullptr);
}

bool DDevice::is_physical_device_suitable(VkPhysicalDevice physical_device)
{
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceFeatures physical_device_features;

    vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

    bool type_ok = (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    bool features_ok = physical_device_features.fillModeNonSolid && physical_device_features.geometryShader;

    QueueFamilyIndices indices = find_queue_families(physical_device);
    bool queues_ok = indices.is_complete();

    bool extension_ok = check_device_extension_support(physical_device);

    auto swapchain = query_swapchain_support(physical_device, surface.get_surface());
    bool swapchain_ok = !swapchain.surface_formats.empty() && !swapchain.present_modes.empty();

    return type_ok && features_ok && queues_ok && extension_ok && swapchain_ok;
}

bool DDevice::check_device_extension_support(VkPhysicalDevice physical_device)
{
    uint32_t count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, available_extensions.data());

    const std::vector<const char *> required_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    std::set<std::string> available_extension_names;
    for (const auto &extension : available_extensions)
        available_extension_names.insert(extension.extensionName);

    for (const char *name : required_extensions)
        if (available_extension_names.find(name) == available_extension_names.end())
            return false;

    return true;
}

QueueFamilyIndices DDevice::find_queue_families(VkPhysicalDevice physical_device)
{
    QueueFamilyIndices result;

    uint32_t count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &count, nullptr);

    std::vector<VkQueueFamilyProperties2> families(count);

    for (auto &family : families)
        family.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;

    vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &count, families.data());

    for (uint32_t index = 0; index < count; ++index)
    {
        if (families[index].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            result.graphics_family = index;

        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface.get_surface(), &present_support);

        if (present_support)
            result.present_family = index;

        if (result.is_complete())
            break;
    }

    return result;
}

SwapchainSupportDetails DDevice::query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.surface_capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);

    if (format_count != 0)
    {
        details.surface_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.surface_formats.data());
    }

    uint32_t present_modes_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_modes_count, nullptr);

    if (present_modes_count != 0)
    {
        details.present_modes.resize(present_modes_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_modes_count, details.present_modes.data());
    }

    return details;
}

void DDevice::select_physical_device()
{
    physical_device = VK_NULL_HANDLE;

    uint32_t count;
    vkEnumeratePhysicalDevices(instance.get_instance(), &count, nullptr);

    if (count == 0)
        throw std::runtime_error(FILE_PREFIX + "No GPUs with Vulkan support found.");

    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(instance.get_instance(), &count, devices.data());

    for (VkPhysicalDevice device : devices)
    {
        if (is_physical_device_suitable(device))
        {
            physical_device = device;
            indices = find_queue_families(device);
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
        throw std::runtime_error(FILE_PREFIX + "No suitable GPU found.");
}

void DDevice::create_logical_device()
{
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_families = {
        indices.graphics_family.value(),
        indices.present_family.value(),
    };

    float priority = 1.0f;
    for (uint32_t family : unique_families)
    {
        VkDeviceQueueCreateInfo queue_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        };

        queue_create_infos.push_back(queue_info);
    }

    VkPhysicalDeviceFeatures2 device_features{};
    device_features.features.fillModeNonSolid = VK_TRUE;
    device_features.features.geometryShader = VK_TRUE;

    const std::vector<const char *> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    VkDeviceCreateInfo logical_device_create_info{};
    logical_device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logical_device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    logical_device_create_info.pQueueCreateInfos = queue_create_infos.data();
    logical_device_create_info.pEnabledFeatures = &device_features.features;
    logical_device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    logical_device_create_info.ppEnabledExtensionNames = device_extensions.data();
    logical_device_create_info.enabledLayerCount = 0;

    VkResult result;
    result = vkCreateDevice(physical_device, &logical_device_create_info, nullptr, &logical_device);

    if (result != VK_SUCCESS)
        throw std::runtime_error(FILE_PREFIX + "Failed to create logical device.");

    vkGetDeviceQueue(logical_device, indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(logical_device, indices.present_family.value(), 0, &present_queue);
}
