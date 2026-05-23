#pragma once

#include "Behavior.h"

class PathfindingBehavior : public Behavior {
public:
    Vector2D calculate(const Robot& robot, const World& world) const override;
    std::string name() const override { return "Pathfinding"; }
    std::unique_ptr<Behavior> clone() const override {
        return std::make_unique<PathfindingBehavior>(*this);
    }
};
