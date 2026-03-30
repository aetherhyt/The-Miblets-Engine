#pragma once

#include "Engine/ECS/Registry.h"
#include <vector>

namespace miblets {

    class TurnManager {
    public:
        TurnManager();
        ~TurnManager() = default;

        void AddEntity(mib::ECS::Entity entity);
        void RemoveEntity(mib::ECS::Entity entity);

        void StartMatch();
        void Update(float deltaTime);
        void EndTurn();

        mib::ECS::Entity GetCurrentEntity() const;
        float GetTurnTimeRemaining() const;

    private:
        std::vector<mib::ECS::Entity> m_entities;
        size_t m_currentTurnIndex = 0;
        float m_turnTimeLimit = 30.0f; // 30 seconds per turn
        float m_currentTurnTime = 0.0f;
        bool m_isMatchActive = false;
    };

}
