#include "PredatorPreyBehavior.h"

#include <limits>

#include "Robot.h"
#include "World.h"

Vector2D PredatorPreyBehavior::calculate(const Robot& robot, const World& world) const {
    const bool predator = robot.symbol() == 'X';
    const Robot* nearest = nullptr;
    double nearestDistance = std::numeric_limits<double>::max();

    for (const Robot& other : world.robots()) {
        if (other.id() == robot.id()) {
            continue;
        }

        const bool otherPredator = other.symbol() == 'X';
        if (predator == otherPredator) {
            continue;
        }

        const double distance = robot.position().distanceTo(other.position());
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearest = &other;
        }
    }

    Vector2D desired;
    if (nearest) {
        const Vector2D direction = predator
            ? nearest->position() - robot.position()
            : robot.position() - nearest->position();
        desired = direction.normalized() * robot.maxSpeed();
    } else {
        desired = (world.target() - robot.position()).normalized() * robot.maxSpeed();
    }

    Vector2D obstacleAvoidance = world.obstacleAvoidanceFrom(robot.position(), 6.0) * robot.maxSpeed();

    const double urgency = predator ? 1.2 : 1.8;
    return (desired * urgency + obstacleAvoidance * 2.5) - robot.velocity();
}
