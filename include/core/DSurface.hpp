#pragma once

#include "core/DInstance.hpp"
#include "core/DWindow.hpp"
#include <vulkan/vulkan_core.h>

#include <string>

namespace dreamsengine
{

    class DSurface
    {
        private:
            static constexpr std::string FILE_PREFIX = "[DSurface] ";

            DInstance& instance;
            DWindow& window;

            VkSurfaceKHR surface;

        public:
            DSurface(DInstance& instance, DWindow& window);
            ~DSurface();

            void create_surface();

            inline VkSurfaceKHR get_surface() const
            {
                return surface;
            }
    };
} // namespace dreamsengine
