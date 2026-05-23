#include "FormationBehavior.h"

#include <cmath>

#include "Robot.h"
#include "World.h"

Vector2D FormationBehavior::calculate(const Robot& robot, const World& world) const {
    constexpr double pi = 3.14159265358979323846;
    const int slot = robot.id() % 12;
    const double ring = 5.0 + static_cast<double>(robot.id() % 3) * 2.0;
    const double angle = (2.0 * pi * static_cast<double>(slot)) / 12.0;
    const Vector2D desiredPosition{
        world.target().x + std::cos(angle) * ring,
        world.target().y + std::sin(angle) * ring
    };

    const Vector2D desiredVelocity = (desiredPosition - robot.position()).normalized() * robot.maxSpeed();
    return desiredVelocity - robot.velocity();
}
