#include "GLFWWindow.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace mib
{
    bool GLFWWindow::Initialize(int width, int height, const std::string& title)
    {
        m_width = width;
        m_height = height;

        if (!glfwInit())
        {
            throw std::runtime_error("glfwInit failed");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed");
        }

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);

        return true;
    }

    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindow::Shutdown()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }

        glfwTerminate();
    }

    bool GLFWWindow::ShouldClose() const
    {
        return m_window ? glfwWindowShouldClose(m_window) : true;
    }

    bool GLFWWindow::IsKeyPressed(int key) const
    {
        return m_window ? (glfwGetKey(m_window, key) == GLFW_PRESS) : false;
    }

    GLFWwindow* GLFWWindow::GetNativeHandle() const
    {
        return m_window;
    }

    void GLFWWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        if (!self)
            return;

        self->m_width = width;
        self->m_height = height;
        self->m_framebufferResized = true;
    }
}
