#pragma once

#include "Behavior.h"

class FlockingBehavior : public Behavior {
public:
    Vector2D calculate(const Robot& robot, const World& world) const override;
    std::string name() const override { return "Flocking"; }
    std::unique_ptr<Behavior> clone() const override {
        return std::make_unique<FlockingBehavior>(*this);
    }

private:
    double perceptionRadius_{12.0};
    double separationRadius_{4.0};
};
