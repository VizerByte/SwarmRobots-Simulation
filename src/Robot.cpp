#include "Robot.h"

#include <stdexcept>

#include "World.h"

Robot::Robot(int id,
             Vector2D position,
             Vector2D velocity,
             std::unique_ptr<Behavior> behavior,
             char symbol)
    : id_(id),
      position_(position),
      velocity_(velocity),
      behavior_(std::move(behavior)),
      symbol_(symbol) {
    if (!behavior_) {
        throw std::invalid_argument("Robot requires a behavior");
    }
}

Robot::Robot(const Robot& other)
    : id_(other.id_),
      position_(other.position_),
      velocity_(other.velocity_),
      behavior_(other.behavior_ ? other.behavior_->clone() : nullptr),
      symbol_(other.symbol_),
      maxSpeed_(other.maxSpeed_),
      maxForce_(other.maxForce_) {}

Robot& Robot::operator=(const Robot& other) {
    if (this == &other) {
        return *this;
    }

    id_ = other.id_;
    position_ = other.position_;
    velocity_ = other.velocity_;
    behavior_ = other.behavior_ ? other.behavior_->clone() : nullptr;
    symbol_ = other.symbol_;
    maxSpeed_ = other.maxSpeed_;
    maxForce_ = other.maxForce_;
    return *this;
}

void Robot::update(const World& world, double deltaTime) {
    const Vector2D steering = behavior_->calculate(*this, world).limited(maxForce_);
    velocity_ += steering * deltaTime;
    velocity_ = velocity_.limited(maxSpeed_);
    position_ += velocity_ * deltaTime;
    position_ = world.wrapped(position_);
}

void Robot::setBehavior(std::unique_ptr<Behavior> behavior) {
    if (!behavior) {
        throw std::invalid_argument("Robot behavior cannot be null");
    }
    behavior_ = std::move(behavior);
}

std::string Robot::behaviorName() const {
    return behavior_ ? behavior_->name() : "None";
}
