#pragma once

#include "core/DInstance.hpp"
#include "core/DSurface.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <string>

namespace dreamsengine
{
    class DDevice
    {
        private:
            static constexpr std::string FILE_PREFIX = "[DDevice] ";

            DInstance& instance;
            DSurface& surface;

            VkPhysicalDevice physical_device;
            VkDevice logical_device;
            VkQueue graphics_queue;
            uint32_t graphics_queue_family;

        public:
            DDevice(DInstance& instance, DSurface& surface);
            ~DDevice();

            bool is_physical_device_suitable(VkPhysicalDevice physical_device);
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
