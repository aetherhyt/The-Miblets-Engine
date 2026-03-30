#pragma once

#include <string>

struct GLFWwindow;

namespace mib
{
    class GLFWWindow
    {
    public:
        GLFWWindow() = default;
        ~GLFWWindow() = default;

        bool Initialize(int width, int height, const std::string& title);
        void PollEvents();
        void Shutdown();

        bool ShouldClose() const;
        bool IsKeyPressed(int key) const;
        GLFWwindow* GetNativeHandle() const;

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        bool WasResized() const { return m_framebufferResized; }
        void ResetResizedFlag() { m_framebufferResized = false; }

    private:
        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_window = nullptr;
        int m_width = 1280;
        int m_height = 720;
        bool m_framebufferResized = false;
    };
}
