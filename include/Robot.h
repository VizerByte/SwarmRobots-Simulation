#pragma once

#include <memory>
#include <string>

#include "Behavior.h"
#include "Vector2D.h"

class World;

class Robot {
public:
    Robot(int id,
          Vector2D position,
          Vector2D velocity,
          std::unique_ptr<Behavior> behavior,
          char symbol = 'R');

    Robot(const Robot& other);
    Robot& operator=(const Robot& other);
    Robot(Robot&&) noexcept = default;
    Robot& operator=(Robot&&) noexcept = default;
    ~Robot() = default;

    void update(const World& world, double deltaTime);
    void setBehavior(std::unique_ptr<Behavior> behavior);

    int id() const { return id_; }
    char symbol() const { return symbol_; }
    const Vector2D& position() const { return position_; }
    const Vector2D& velocity() const { return velocity_; }
    double maxSpeed() const { return maxSpeed_; }
    double maxForce() const { return maxForce_; }
    std::string behaviorName() const;

    void setPosition(const Vector2D& position) { position_ = position; }
    void setVelocity(const Vector2D& velocity) { velocity_ = velocity; }
    void setMaxSpeed(double maxSpeed) { maxSpeed_ = maxSpeed; }
    void setMaxForce(double maxForce) { maxForce_ = maxForce; }

private:
    int id_{0};
    Vector2D position_;
    Vector2D velocity_;
    std::unique_ptr<Behavior> behavior_;
    char symbol_{'R'};
    double maxSpeed_{9.0};
    double maxForce_{8.0};
};
