#include "World.h"

#include <algorithm>
#include <cmath>

World::World(double width, double height)
    : width_(std::max(10.0, width)),
      height_(std::max(10.0, height)),
      target_(width_ * 0.5, height_ * 0.5) {}

Robot& World::addRobot(std::unique_ptr<Behavior> behavior,
                       Vector2D position,
                       Vector2D velocity,
                       char symbol) {
    robots_.emplace_back(nextRobotId_++, wrapped(position), velocity, std::move(behavior), symbol);
    return robots_.back();
}

void World::update(double deltaTime) {
    for (Robot& robot : robots_) {
        robot.update(*this, deltaTime);
        resolveRobotCollision(robot);
    }

    resolveRobotRobotCollisions();

    for (Robot& robot : robots_) {
        resolveRobotCollision(robot);
    }
}

std::vector<const Robot*> World::neighborsOf(const Robot& robot, double radius) const {
    std::vector<const Robot*> result;
    const double radiusSquared = radius * radius;

    for (const Robot& other : robots_) {
        if (other.id() == robot.id()) {
            continue;
        }

        const Vector2D offset = other.position() - robot.position();
        if (offset.lengthSquared() <= radiusSquared) {
            result.push_back(&other);
        }
    }

    return result;
}

Vector2D World::wrapped(Vector2D position) const {
    while (position.x < 0.0) {
        position.x += width_;
    }
    while (position.y < 0.0) {
        position.y += height_;
    }
    while (position.x >= width_) {
        position.x -= width_;
    }
    while (position.y >= height_) {
        position.y -= height_;
    }
    return position;
}

Vector2D World::clamped(Vector2D position) const {
    position.x = std::clamp(position.x, 0.0, width_ - 1.0);
    position.y = std::clamp(position.y, 0.0, height_ - 1.0);
    return position;
}

void World::randomizeTarget(std::mt19937& rng) {
    std::uniform_real_distribution<double> xDistribution(4.0, width_ - 5.0);
    std::uniform_real_distribution<double> yDistribution(3.0, height_ - 4.0);
    target_ = {xDistribution(rng), yDistribution(rng)};
}

void World::addObstacle(Vector2D obstacle) {
    addRock(obstacle, 2.2);
}

void World::addRock(Vector2D center, double radius) {
    obstacles_.push_back({ObstacleType::Rock, clamped(center), {}, std::max(0.5, radius)});
}

void World::addWall(Vector2D topLeft, Vector2D size) {
    const Vector2D safeSize{std::max(1.0, size.x), std::max(1.0, size.y)};
    const Vector2D safeTopLeft{
        std::clamp(topLeft.x, 0.0, std::max(0.0, width_ - safeSize.x)),
        std::clamp(topLeft.y, 0.0, std::max(0.0, height_ - safeSize.y))
    };
    obstacles_.push_back({ObstacleType::Wall, safeTopLeft, safeSize, 0.0});
}

Vector2D World::nearestPointOnObstacle(const Obstacle& obstacle, Vector2D point) const {
    if (obstacle.type == ObstacleType::Rock) {
        const Vector2D fromCenter = point - obstacle.position;
        if (fromCenter.lengthSquared() <= 1e-9) {
            return obstacle.position + Vector2D{obstacle.radius, 0.0};
        }
        return obstacle.position + fromCenter.normalized() * obstacle.radius;
    }

    return {
        std::clamp(point.x, obstacle.position.x, obstacle.position.x + obstacle.size.x),
        std::clamp(point.y, obstacle.position.y, obstacle.position.y + obstacle.size.y)
    };
}

Vector2D World::obstacleAvoidanceFrom(Vector2D point, double influenceRadius) const {
    Vector2D avoidance;

    for (const Obstacle& obstacle : obstacles_) {
        const Vector2D nearest = nearestPointOnObstacle(obstacle, point);
        Vector2D away = point - nearest;
        double distance = away.length();

        if (obstacle.type == ObstacleType::Wall &&
            point.x >= obstacle.position.x && point.x <= obstacle.position.x + obstacle.size.x &&
            point.y >= obstacle.position.y && point.y <= obstacle.position.y + obstacle.size.y) {
            const double left = point.x - obstacle.position.x;
            const double right = obstacle.position.x + obstacle.size.x - point.x;
            const double top = point.y - obstacle.position.y;
            const double bottom = obstacle.position.y + obstacle.size.y - point.y;
            const double minimum = std::min(std::min(left, right), std::min(top, bottom));

            if (minimum == left) {
                away = {-1.0, 0.0};
            } else if (minimum == right) {
                away = {1.0, 0.0};
            } else if (minimum == top) {
                away = {0.0, -1.0};
            } else {
                away = {0.0, 1.0};
            }
            distance = 0.0;
        }

        if (distance < influenceRadius) {
            const double strength = (influenceRadius - distance) / influenceRadius;
            avoidance += away.normalized() * strength;
        }
    }

    return avoidance;
}

int World::obstacleCount(ObstacleType type) const {
    return static_cast<int>(std::count_if(obstacles_.begin(), obstacles_.end(), [type](const Obstacle& obstacle) {
        return obstacle.type == type;
    }));
}

double World::robotCollisionRadius(const Robot& robot) const {
    return robot.symbol() == 'X' ? 0.58 : 0.48;
}

double World::robotSeparationRadius(const Robot& robot) const {
    return robot.symbol() == 'X' ? 0.95 : 0.88;
}

void World::resolveRobotCollision(Robot& robot) const {
    const double robotRadius = robotCollisionRadius(robot);
    Vector2D position = robot.position();
    Vector2D velocity = robot.velocity();

    for (const Obstacle& obstacle : obstacles_) {
        if (obstacle.type == ObstacleType::Rock) {
            Vector2D away = position - obstacle.position;
            double distance = away.length();
            const double minimumDistance = obstacle.radius + robotRadius;

            if (distance < minimumDistance) {
                Vector2D normal = distance > 1e-9 ? away.normalized() : Vector2D{1.0, 0.0};
                position = obstacle.position + normal * minimumDistance;
                const double intoObstacle = velocity.x * normal.x + velocity.y * normal.y;
                if (intoObstacle < 0.0) {
                    velocity -= normal * (1.65 * intoObstacle);
                }
            }
            continue;
        }

        const double left = obstacle.position.x;
        const double right = obstacle.position.x + obstacle.size.x;
        const double top = obstacle.position.y;
        const double bottom = obstacle.position.y + obstacle.size.y;
        const double nearestX = std::clamp(position.x, left, right);
        const double nearestY = std::clamp(position.y, top, bottom);
        Vector2D away = position - Vector2D{nearestX, nearestY};
        double distance = away.length();

        if (position.x >= left && position.x <= right && position.y >= top && position.y <= bottom) {
            const double toLeft = std::abs(position.x - left);
            const double toRight = std::abs(right - position.x);
            const double toTop = std::abs(position.y - top);
            const double toBottom = std::abs(bottom - position.y);
            const double minimum = std::min(std::min(toLeft, toRight), std::min(toTop, toBottom));

            if (minimum == toLeft) {
                away = {-1.0, 0.0};
                distance = -toLeft;
            } else if (minimum == toRight) {
                away = {1.0, 0.0};
                distance = -toRight;
            } else if (minimum == toTop) {
                away = {0.0, -1.0};
                distance = -toTop;
            } else {
                away = {0.0, 1.0};
                distance = -toBottom;
            }
        }

        if (distance < robotRadius) {
            const Vector2D normal = away.lengthSquared() > 1e-9 ? away.normalized() : Vector2D{1.0, 0.0};
            position += normal * (robotRadius - distance);
            const double intoObstacle = velocity.x * normal.x + velocity.y * normal.y;
            if (intoObstacle < 0.0) {
                velocity -= normal * (1.65 * intoObstacle);
            }
        }
    }

    robot.setPosition(wrapped(position));
    robot.setVelocity(velocity.limited(robot.maxSpeed()));
}

void World::resolveRobotRobotCollisions() {
    constexpr int iterations = 2;

    for (int iteration = 0; iteration < iterations; ++iteration) {
        for (size_t i = 0; i < robots_.size(); ++i) {
            for (size_t j = i + 1; j < robots_.size(); ++j) {
                Robot& first = robots_[i];
                Robot& second = robots_[j];

                Vector2D offset = second.position() - first.position();
                double distance = offset.length();
                const double minimumDistance = robotSeparationRadius(first) + robotSeparationRadius(second);

                if (distance >= minimumDistance) {
                    continue;
                }

                if (distance <= 1e-9) {
                    offset = {1.0, 0.0};
                    distance = 1.0;
                }

                const Vector2D normal = offset.normalized();
                const double overlap = minimumDistance - distance;
                first.setPosition(wrapped(first.position() - normal * (overlap * 0.5)));
                second.setPosition(wrapped(second.position() + normal * (overlap * 0.5)));

                const Vector2D firstVelocity = first.velocity();
                const Vector2D secondVelocity = second.velocity();
                const Vector2D relativeVelocity = secondVelocity - firstVelocity;
                const double closingSpeed = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

                if (closingSpeed < 0.0) {
                    const Vector2D impulse = normal * (closingSpeed * 0.35);
                    first.setVelocity((firstVelocity + impulse).limited(first.maxSpeed()));
                    second.setVelocity((secondVelocity - impulse).limited(second.maxSpeed()));
                }
            }
        }
    }
}
