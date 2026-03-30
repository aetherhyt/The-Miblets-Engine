#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <glm/glm.hpp>
#include "../UI/NoesisSystem.h"

namespace mib
{
    class GLFWWindow;
    class VulkanContext;
    class Renderer;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        bool Initialize();
        void Run();
        void Shutdown();
        void Quit();

        bool IsKeyPressed(int key) const;

        // Game Layer Hook
        void SetGameUpdateCallback(std::function<void(float)> callback) { m_gameUpdateCallback = callback; }
        
        void SetRenderables(const std::vector<glm::mat4>& models) { m_renderables = models; }

    private:
        bool m_running = false;
        std::function<void(float)> m_gameUpdateCallback;
        std::vector<glm::mat4> m_renderables;

        std::unique_ptr<GLFWWindow> m_window;
        std::unique_ptr<VulkanContext> m_vulkan;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<UI::NoesisSystem> m_uiSystem;
    };
}
