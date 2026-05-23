#include "PathfindingBehavior.h"

#include "Robot.h"
#include "World.h"

Vector2D PathfindingBehavior::calculate(const Robot& robot, const World& world) const {
    Vector2D desired = (world.target() - robot.position()).normalized() * robot.maxSpeed();
    Vector2D avoidance = world.obstacleAvoidanceFrom(robot.position(), 2.7) * robot.maxSpeed();

    desired += avoidance * 1.35;
    return desired - robot.velocity();
}
