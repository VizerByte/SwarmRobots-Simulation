#pragma once

#include <random>
#include <vector>

#include "Robot.h"
#include "Vector2D.h"

class World {
public:
    enum class ObstacleType {
        Rock,
        Wall
    };

    struct Obstacle {
        ObstacleType type{ObstacleType::Rock};
        Vector2D position;
        Vector2D size;
        double radius{1.0};
    };

    World(double width, double height);

    Robot& addRobot(std::unique_ptr<Behavior> behavior,
                    Vector2D position,
                    Vector2D velocity = {},
                    char symbol = 'R');

    void update(double deltaTime);
    std::vector<const Robot*> neighborsOf(const Robot& robot, double radius) const;
    Vector2D wrapped(Vector2D position) const;
    Vector2D clamped(Vector2D position) const;

    double width() const { return width_; }
    double height() const { return height_; }

    const Vector2D& target() const { return target_; }
    void setTarget(Vector2D target) { target_ = clamped(target); }
    void randomizeTarget(std::mt19937& rng);

    void addObstacle(Vector2D obstacle);
    void addRock(Vector2D center, double radius);
    void addWall(Vector2D topLeft, Vector2D size);
    void clearObstacles() { obstacles_.clear(); }
    Vector2D nearestPointOnObstacle(const Obstacle& obstacle, Vector2D point) const;
    Vector2D obstacleAvoidanceFrom(Vector2D point, double influenceRadius) const;
    int obstacleCount(ObstacleType type) const;
    const std::vector<Obstacle>& obstacles() const { return obstacles_; }

    const std::vector<Robot>& robots() const { return robots_; }
    std::vector<Robot>& robots() { return robots_; }

private:
    double width_{80.0};
    double height_{25.0};
    int nextRobotId_{1};
    Vector2D target_;
    std::vector<Obstacle> obstacles_;
    std::vector<Robot> robots_;

    double robotCollisionRadius(const Robot& robot) const;
    double robotSeparationRadius(const Robot& robot) const;
    void resolveRobotCollision(Robot& robot) const;
    void resolveRobotRobotCollisions();
};
