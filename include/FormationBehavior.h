#pragma once

#include "Behavior.h"

class FormationBehavior : public Behavior {
public:
    Vector2D calculate(const Robot& robot, const World& world) const override;
    std::string name() const override { return "Formation"; }
    std::unique_ptr<Behavior> clone() const override {
        return std::make_unique<FormationBehavior>(*this);
    }
};
