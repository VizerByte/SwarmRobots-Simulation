#include "DemoWorld.h"

#include <memory>
#include <random>

#include "FlockingBehavior.h"
#include "FormationBehavior.h"
#include "PathfindingBehavior.h"
#include "PredatorPreyBehavior.h"
#include "Robot.h"
#include "World.h"

void seedDemoWorld(World& world, std::mt19937& rng) {
    std::uniform_real_distribution<double> xDistribution(0.0, world.width() - 1.0);
    std::uniform_real_distribution<double> yDistribution(0.0, world.height() - 1.0);
    std::uniform_real_distribution<double> velocityDistribution(-1.0, 1.0);

    world.setTarget({world.width() * 0.68, world.height() * 0.5});
    world.addRock({world.width() * 0.35, world.height() * 0.35}, 2.2);
    world.addRock({world.width() * 0.48, world.height() * 0.65}, 2.5);
    world.addRock({world.width() * 0.62, world.height() * 0.32}, 2.0);
    world.addWall({world.width() * 0.18, world.height() * 0.52}, {12.0, 1.5});
    world.addWall({world.width() * 0.72, world.height() * 0.58}, {1.6, 12.0});

    for (int i = 0; i < 12; ++i) {
        world.addRobot(
            std::make_unique<FlockingBehavior>(),
            {xDistribution(rng), yDistribution(rng)},
            {velocityDistribution(rng), velocityDistribution(rng)},
            'R');
    }

    for (int i = 0; i < 8; ++i) {
        Robot& robot = world.addRobot(
            std::make_unique<FormationBehavior>(),
            {xDistribution(rng), yDistribution(rng)},
            {velocityDistribution(rng), velocityDistribution(rng)},
            'F');
        robot.setMaxSpeed(7.0);
    }

    for (int i = 0; i < 5; ++i) {
        Robot& robot = world.addRobot(
            std::make_unique<PathfindingBehavior>(),
            {xDistribution(rng), yDistribution(rng)},
            {velocityDistribution(rng), velocityDistribution(rng)},
            'P');
        robot.setMaxSpeed(8.0);
    }

    for (int i = 0; i < 2; ++i) {
        Robot& robot = world.addRobot(
            std::make_unique<PredatorPreyBehavior>(),
            {xDistribution(rng), yDistribution(rng)},
            {velocityDistribution(rng), velocityDistribution(rng)},
            'X');
        robot.setMaxSpeed(10.5);
        robot.setMaxForce(10.0);
    }
}
