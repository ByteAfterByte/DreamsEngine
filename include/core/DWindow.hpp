#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace dreamsengine
{
    static constexpr std::string FILE_PREFIX = "[DWindow] ";

    class DWindow
    {
        private:
            GLFWwindow* window;

        public:
            DWindow();
            ~DWindow();

            void create_window(unsigned int width, unsigned int height, const char* title);

            inline GLFWwindow* get_window() const
            {
                return window;
            }
    };
}
