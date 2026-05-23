#pragma once

#include "Behavior.h"

class PredatorPreyBehavior : public Behavior {
public:
    Vector2D calculate(const Robot& robot, const World& world) const override;
    std::string name() const override { return "PredatorPrey"; }
    std::unique_ptr<Behavior> clone() const override {
        return std::make_unique<PredatorPreyBehavior>(*this);
    }
};
