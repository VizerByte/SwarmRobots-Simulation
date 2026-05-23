#include "FlockingBehavior.h"

#include "Robot.h"
#include "World.h"

namespace {
Vector2D steerToward(const Vector2D& desired, const Robot& robot) {
    if (desired.lengthSquared() <= 1e-9) {
        return {};
    }
    return desired.normalized() * robot.maxSpeed() - robot.velocity();
}
}

Vector2D FlockingBehavior::calculate(const Robot& robot, const World& world) const {
    Vector2D separation;
    Vector2D alignment;
    Vector2D cohesion;
    int neighborCount = 0;

    for (const Robot* neighbor : world.neighborsOf(robot, perceptionRadius_)) {
        const Vector2D toRobot = robot.position() - neighbor->position();
        const double distance = toRobot.length();

        if (distance > 1e-9 && distance < separationRadius_) {
            separation += toRobot.normalized() / distance;
        }

        alignment += neighbor->velocity();
        cohesion += neighbor->position();
        ++neighborCount;
    }

    if (neighborCount == 0) {
        return steerToward(world.target() - robot.position(), robot) * 0.35;
    }

    alignment = alignment / static_cast<double>(neighborCount);
    cohesion = (cohesion / static_cast<double>(neighborCount)) - robot.position();

    const Vector2D separationSteer = steerToward(separation, robot) * 1.8;
    const Vector2D alignmentSteer = steerToward(alignment, robot) * 0.8;
    const Vector2D cohesionSteer = steerToward(cohesion, robot) * 0.7;
    const Vector2D targetSteer = steerToward(world.target() - robot.position(), robot) * 0.15;

    return separationSteer + alignmentSteer + cohesionSteer + targetSteer;
}
