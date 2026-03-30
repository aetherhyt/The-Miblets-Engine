#pragma once

#include "Engine/Core/Engine.h"
#include "Engine/ECS/Registry.h"
#include "TurnManager.h"
#include <memory>

namespace miblets {

    enum class GameState {
        MainMenu,
        Options,
        Playing
    };

    class MibletsApp {
    public:
        MibletsApp();
        ~MibletsApp();

        bool Initialize();
        void Run();
        void Shutdown();

    private:
        void Update(float deltaTime);
        void SetupLevel();
        void DrawMainMenu();
        void DrawOptionsMenu();

    private:
        std::unique_ptr<mib::Engine> m_engine;
        std::unique_ptr<mib::ECS::Registry> m_registry;
        std::unique_ptr<TurnManager> m_turnManager;
        
        // System references or game state
        float m_gameTime = 0.0f;
        GameState m_currentState = GameState::MainMenu;
        int m_optionsTab = 0; // 0 = Video, 1 = Audio, 2 = Gameplay
    };

}