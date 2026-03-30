#pragma once

#include <string>
#include <memory>

#ifdef USING_NOESIS
#include <NsCore/Noesis.h>
#include <NsGui/IView.h>
#endif

namespace mib {
    class VulkanContext;

    namespace UI {

        class NoesisSystem {
        public:
            NoesisSystem();
            ~NoesisSystem();

            // Initialize the UI System with context
            bool Initialize(VulkanContext* vulkanContext);
            
            // Load a XAML view
            void LoadHUD(const std::string& xamlPath);

            // Update UI constraints (animations, layout, input)
            void Update(float deltaTime);

            // Render XAML to current Vulkan Command Buffer
            void Render();

            void Shutdown();

        private:
#ifdef USING_NOESIS
            Noesis::Ptr<Noesis::IView> m_view;
#endif
            
            VulkanContext* m_vulkanContext;
            bool m_isInitialized = false;
        };

    } // namespace UI
} // namespace mib