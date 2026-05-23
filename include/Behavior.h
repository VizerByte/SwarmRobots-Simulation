#pragma once

#include <memory>
#include <string>

#include "Vector2D.h"

class Robot;
class World;

class Behavior {
public:
    virtual ~Behavior() = default;

    virtual Vector2D calculate(const Robot& robot, const World& world) const = 0;
    virtual std::string name() const = 0;
    virtual std::unique_ptr<Behavior> clone() const = 0;
};
