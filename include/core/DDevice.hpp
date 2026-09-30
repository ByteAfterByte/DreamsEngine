#pragma once

#include "core/DInstance.hpp"
#include "core/DSurface.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <string>
#include <optional>
#include <vector>

namespace dreamsengine
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_complete() const
        {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR surface_capabilities;
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class DDevice
    {
    private:
        static constexpr std::string FILE_PREFIX = "[DDevice] ";

        DInstance &instance;
        DSurface &surface;

        VkPhysicalDevice physical_device;
        VkDevice logical_device;

        QueueFamilyIndices indices;
        VkQueue graphics_queue;
        VkQueue present_queue;
        uint32_t graphics_queue_family;

    public:
        DDevice(DInstance &instance, DSurface &surface);
        ~DDevice();

        bool is_physical_device_suitable(VkPhysicalDevice physical_device);
        bool check_device_extension_support(VkPhysicalDevice physical_device);
        QueueFamilyIndices find_queue_families(VkPhysicalDevice physical_device);
        SwapchainSupportDetails query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void select_physical_device();

        inline VkPhysicalDevice get_physical_device() const
        {
            return physical_device;
        }

        void create_logical_device();

        VkDevice get_logical_device() const
        {
            return logical_device;
        }
    };
}
