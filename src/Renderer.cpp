#include "Renderer.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "World.h"

Renderer::Renderer(int width, int height)
    : width_(std::max(10, width)),
      height_(std::max(10, height)) {}

void Renderer::draw(const World& world, int step, std::ostream& out) const {
    std::vector<std::string> grid(static_cast<size_t>(height_), std::string(static_cast<size_t>(width_), '.'));

    for (const World::Obstacle& obstacle : world.obstacles()) {
        if (obstacle.type == World::ObstacleType::Rock) {
            const int x = std::clamp(static_cast<int>(obstacle.position.x), 0, width_ - 1);
            const int y = std::clamp(static_cast<int>(obstacle.position.y), 0, height_ - 1);
            grid[static_cast<size_t>(y)][static_cast<size_t>(x)] = '#';
            continue;
        }

        const int left = std::clamp(static_cast<int>(obstacle.position.x), 0, width_ - 1);
        const int top = std::clamp(static_cast<int>(obstacle.position.y), 0, height_ - 1);
        const int right = std::clamp(static_cast<int>(obstacle.position.x + obstacle.size.x), 0, width_ - 1);
        const int bottom = std::clamp(static_cast<int>(obstacle.position.y + obstacle.size.y), 0, height_ - 1);

        for (int y = top; y <= bottom; ++y) {
            for (int x = left; x <= right; ++x) {
                grid[static_cast<size_t>(y)][static_cast<size_t>(x)] = '=';
            }
        }
    }

    const int targetX = std::clamp(static_cast<int>(world.target().x), 0, width_ - 1);
    const int targetY = std::clamp(static_cast<int>(world.target().y), 0, height_ - 1);
    grid[static_cast<size_t>(targetY)][static_cast<size_t>(targetX)] = '*';

    for (const Robot& robot : world.robots()) {
        const int x = std::clamp(static_cast<int>(robot.position().x), 0, width_ - 1);
        const int y = std::clamp(static_cast<int>(robot.position().y), 0, height_ - 1);
        grid[static_cast<size_t>(y)][static_cast<size_t>(x)] = robot.symbol();
    }

    out << "Step " << std::setw(4) << step
        << " | robots: " << world.robots().size()
        << " | target: " << world.target()
        << "\n";
    out << "Legend: R flocking, F formation, P pathfinding, X predator, * target, # rock, = wall\n";

    for (const std::string& row : grid) {
        out << row << '\n';
    }
}
