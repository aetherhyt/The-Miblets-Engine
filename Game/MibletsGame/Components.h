#pragma once

#include <glm/glm.hpp>
#include <string>

namespace miblets {

    struct ProjectileComponent {
        glm::vec3 Velocity{0.0f};
        float Mass = 1.0f;
        float DamageRadius = 5.0f;
        float BaseDamage = 25.0f;
        bool Active = true;
    };

    struct WeaponComponent {
        std::string Name = "Bazooka";
        float BaseDamage = 25.0f;
        int Ammo = -1; // -1 for infinite
        float FirePower = 10.0f; // Multiplier for initial projectile velocity
    };

}
