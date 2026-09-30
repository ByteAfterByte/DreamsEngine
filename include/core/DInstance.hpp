#pragma once

#include <string>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace dreamsengine
{
    class DInstance
    {
        private:
            constexpr static std::string FILE_PREFIX = "[DInstance] ";
            VkInstance instance;

        public:
            DInstance();
            ~DInstance();

            void create_instance();

            inline VkInstance get_instance() const
            {
                return instance;
            }

    };
} // namespace dreamsengine
