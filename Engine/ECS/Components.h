#pragma once

#include <string>
#include <glm/glm.hpp>

namespace mib::ECS
{
    struct TransformComponent
    {
        glm::vec3 Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale    = {1.0f, 1.0f, 1.0f};
    };

    struct MeshComponent
    {
        std::string MeshPath;
        std::string MaterialName;
    };

    struct MibletComponent
    {
        std::string Name = "Unnamed Miblet";
        float Health = 100.0f;
    };
}
