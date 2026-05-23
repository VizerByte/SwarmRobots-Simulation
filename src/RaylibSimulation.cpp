#include "RaylibSimulation.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "PathfindingBehavior.h"
#include "World.h"

#include "raylib.h"

namespace {
constexpr int initialScreenWidth = 1280;
constexpr int initialScreenHeight = 760;
constexpr int minimumScreenWidth = 860;
constexpr int minimumScreenHeight = 520;
constexpr int panelWidth = 340;
constexpr int panelContentHeight = 980;
constexpr double logicalWorldWidth = 80.0;
constexpr double logicalWorldHeight = 50.0;

enum class EnvironmentMode {
    Sandbox,
    Maze
};

struct Button {
    Rectangle bounds{};
    const char* label{""};
    bool selected{false};
};

struct Settings {
    bool running{true};
    bool clickTarget{true};
    double speed{1.0};
    double swarmEnergy{1.25};
    int robotCount{34};
    int rockCount{5};
    int wallCount{3};
    int step{0};
    int mazeCheckpoint{0};
    float panelScroll{0.0f};
    EnvironmentMode environment{EnvironmentMode::Sandbox};
};

struct Stats {
    double averageDistance{0.0};
    double nearestPair{0.0};
    double averageSpeed{0.0};
};

int currentWorldWidth() {
    return std::max(360, GetScreenWidth() - panelWidth);
}

int currentWorldHeight() {
    return std::max(320, GetScreenHeight());
}

float panelY(const Settings& settings, float y) {
    return y - settings.panelScroll;
}

const char* environmentName(EnvironmentMode environment) {
    return environment == EnvironmentMode::Sandbox ? "Sandbox" : "Maze";
}

Rectangle panelViewport() {
    return Rectangle{
        static_cast<float>(currentWorldWidth()),
        0.0f,
        static_cast<float>(panelWidth),
        static_cast<float>(GetScreenHeight())
    };
}

Color robotColor(char symbol) {
    (void)symbol;
    return Color{31, 118, 209, 255};
}

float scaleX(const World& world) {
    return static_cast<float>(currentWorldWidth() / world.width());
}

float scaleY(const World& world) {
    return static_cast<float>(currentWorldHeight() / world.height());
}

float uniformScale(const World& world) {
    return std::min(scaleX(world), scaleY(world));
}

Vector2 toScreen(const World& world, const Vector2D& point) {
    return {
        static_cast<float>(point.x) * scaleX(world),
        static_cast<float>(point.y) * scaleY(world)
    };
}

Vector2D toWorldPoint(const World& world, Vector2 point) {
    return {
        static_cast<double>(point.x) / scaleX(world),
        static_cast<double>(point.y) / scaleY(world)
    };
}

bool intersectsPanel(const Rectangle& bounds) {
    const Rectangle viewport = panelViewport();
    return CheckCollisionRecs(bounds, viewport);
}

bool drawButton(const Button& button) {
    if (!intersectsPanel(button.bounds)) {
        return false;
    }

    const Vector2 mouse = GetMousePosition();
    const bool hover = CheckCollisionPointRec(mouse, button.bounds);
    const Color fill = button.selected
        ? Color{215, 235, 226, 255}
        : (hover ? Color{226, 234, 229, 255} : Color{246, 248, 246, 255});
    const Color border = button.selected
        ? Color{47, 125, 96, 255}
        : (hover ? Color{47, 125, 96, 255} : Color{187, 197, 190, 255});
    const Color text = button.selected ? Color{24, 96, 69, 255} : Color{36, 45, 42, 255};

    DrawRectangleRounded(button.bounds, 0.16f, 8, fill);
    DrawRectangleLinesEx(button.bounds, 1.0f, border);

    const int fontSize = 17;
    const int textWidth = MeasureText(button.label, fontSize);
    DrawText(button.label,
             static_cast<int>(button.bounds.x + (button.bounds.width - textWidth) * 0.5f),
             static_cast<int>(button.bounds.y + 11.0f),
             fontSize,
             text);

    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void drawGrid() {
    ClearBackground(Color{249, 250, 248, 255});
    const int width = currentWorldWidth();
    const int height = currentWorldHeight();

    for (int x = 0; x < width; x += 38) {
        DrawLine(x, 0, x, height, Color{218, 225, 220, 255});
    }
    for (int y = 0; y < height; y += 38) {
        DrawLine(0, y, width, y, Color{218, 225, 220, 255});
    }
    DrawRectangleLines(0, 0, width, height, Color{169, 181, 173, 255});
}

void drawTarget(const World& world) {
    const Vector2 target = toScreen(world, world.target());
    const int x = static_cast<int>(target.x);
    const int y = static_cast<int>(target.y);
    const Color color{37, 88, 142, 255};

    DrawCircle(x, y, 5.0f, color);
    DrawCircleLines(x, y, 18.0f, color);
    DrawLine(x - 26, y, x + 26, y, color);
    DrawLine(x, y - 26, x, y + 26, color);
}

std::vector<Vector2D> mazeCheckpoints(const World& world) {
    return {
        {6.0, world.height() - 6.0},
        {18.0, world.height() - 6.0},
        {18.0, 7.0},
        {34.0, 7.0},
        {34.0, 27.0},
        {50.0, 27.0},
        {50.0, 9.0},
        {66.0, 9.0},
        {66.0, 41.0},
        {world.width() - 6.0, 41.0}
    };
}

void drawMazeRoute(const World& world, const Settings& settings) {
    if (settings.environment != EnvironmentMode::Maze) {
        return;
    }

    const std::vector<Vector2D> checkpoints = mazeCheckpoints(world);
    for (size_t i = 0; i < checkpoints.size(); ++i) {
        const Vector2 point = toScreen(world, checkpoints[i]);
        const bool active = static_cast<int>(i) == settings.mazeCheckpoint;
        const bool complete = static_cast<int>(i) < settings.mazeCheckpoint;
        const Color color = active
            ? Color{37, 88, 142, 180}
            : (complete ? Color{47, 125, 96, 115} : Color{137, 148, 142, 95});

        DrawCircleV(point, active ? 6.0f : 4.0f, color);

        if (i > 0) {
            const Vector2 previous = toScreen(world, checkpoints[i - 1]);
            DrawLineEx(previous, point, 2.0f, Color{92, 105, 98, 75});
        }
    }
}

void drawObstacles(const World& world) {
    const float scale = uniformScale(world);

    for (const World::Obstacle& obstacle : world.obstacles()) {
        if (obstacle.type == World::ObstacleType::Rock) {
            const Vector2 point = toScreen(world, obstacle.position);
            const float radius = static_cast<float>(obstacle.radius) * scale;
            DrawCircle(static_cast<int>(point.x), static_cast<int>(point.y), radius, Color{126, 104, 83, 255});
            DrawCircleLines(static_cast<int>(point.x), static_cast<int>(point.y), radius + 2.0f, Color{79, 68, 58, 255});
            DrawCircle(static_cast<int>(point.x - radius * 0.25f), static_cast<int>(point.y - radius * 0.25f), radius * 0.25f, Color{155, 134, 110, 255});
            continue;
        }

        const Vector2 position = toScreen(world, obstacle.position);
        const Rectangle wall{
            position.x,
            position.y,
            static_cast<float>(obstacle.size.x) * scaleX(world),
            static_cast<float>(obstacle.size.y) * scaleY(world)
        };
        DrawRectangleRounded(wall, 0.12f, 8, Color{118, 128, 138, 255});
        DrawRectangleRoundedLines(wall, 0.12f, 8, Color{72, 83, 94, 255});
    }
}

void drawRobot(const World& world, const Robot& robot) {
    const Vector2D& velocity = robot.velocity();
    const float angle = static_cast<float>(std::atan2(velocity.y, velocity.x));
    const float radius = 12.0f;
    const Vector2 point = toScreen(world, robot.position());
    const float x = point.x;
    const float y = point.y;
    const Color color = robotColor(robot.symbol());

    const Vector2 nose{
        x + std::cos(angle) * (radius + 14.0f),
        y + std::sin(angle) * (radius + 14.0f)
    };
    const Vector2 left{
        x + std::cos(angle + 2.48f) * radius,
        y + std::sin(angle + 2.48f) * radius
    };
    const Vector2 right{
        x + std::cos(angle - 2.48f) * radius,
        y + std::sin(angle - 2.48f) * radius
    };

    DrawCircleV(point, radius * 0.82f, Fade(color, 0.92f));
    DrawTriangle(nose, right, left, color);
    DrawTriangleLines(nose, left, right, Color{34, 41, 38, 180});
    DrawCircle(static_cast<int>(x), static_cast<int>(y), 3.0f, Color{255, 255, 255, 245});
}

Stats calculateStats(const World& world) {
    Stats stats;
    if (world.robots().empty()) {
        return stats;
    }

    double totalDistance = 0.0;
    double totalSpeed = 0.0;
    double nearest = 999999.0;

    for (size_t i = 0; i < world.robots().size(); ++i) {
        totalDistance += world.robots()[i].position().distanceTo(world.target());
        totalSpeed += world.robots()[i].velocity().length();

        for (size_t j = i + 1; j < world.robots().size(); ++j) {
            nearest = std::min(nearest, world.robots()[i].position().distanceTo(world.robots()[j].position()));
        }
    }

    stats.averageDistance = totalDistance / static_cast<double>(world.robots().size());
    stats.averageSpeed = totalSpeed / static_cast<double>(world.robots().size());
    stats.nearestPair = nearest == 999999.0 ? 0.0 : nearest;
    return stats;
}

void drawStatCard(int x, int y, const char* label, const char* value) {
    const Rectangle bounds{static_cast<float>(x), static_cast<float>(y), 136.0f, 58.0f};
    if (!intersectsPanel(bounds)) {
        return;
    }

    DrawRectangleRounded(bounds, 0.12f, 8, Color{255, 255, 255, 255});
    DrawRectangleLinesEx(bounds, 1.0f, Color{204, 213, 207, 255});
    DrawText(label, x + 10, y + 9, 14, Color{101, 113, 106, 255});
    DrawText(value, x + 10, y + 30, 20, Color{32, 41, 37, 255});
}

void drawLabel(int x, int y, const char* text) {
    if (y < -24 || y > GetScreenHeight() + 24) {
        return;
    }
    DrawText(text, x, y, 16, Color{74, 87, 80, 255});
}

void drawPanelBackground() {
    const int left = currentWorldWidth();
    const int height = GetScreenHeight();

    DrawRectangle(left, 0, panelWidth, height, Color{238, 243, 239, 255});
    DrawLine(left, 0, left, height, Color{194, 205, 197, 255});
}

void drawScrollbar(const Settings& settings) {
    const int left = currentWorldWidth();
    const int height = GetScreenHeight();
    const float maxScroll = std::max(0.0f, static_cast<float>(panelContentHeight - height));

    if (maxScroll <= 1.0f) {
        return;
    }

    const float trackX = static_cast<float>(left + panelWidth - 10);
    const float trackHeight = static_cast<float>(height - 24);
    const float thumbHeight = std::max(64.0f, trackHeight * (static_cast<float>(height) / panelContentHeight));
    const float thumbY = 12.0f + (trackHeight - thumbHeight) * (settings.panelScroll / maxScroll);

    DrawRectangleRounded({trackX, 12.0f, 4.0f, trackHeight}, 1.0f, 4, Color{211, 220, 214, 255});
    DrawRectangleRounded({trackX - 2.0f, thumbY, 8.0f, thumbHeight}, 1.0f, 4, Color{102, 122, 111, 255});
}

void drawPanelContent(const World& world, const Settings& settings) {
    const int left = currentWorldWidth();
    const Stats stats = calculateStats(world);

    BeginScissorMode(left, 0, panelWidth, GetScreenHeight());

    DrawText("Swarm Lab", left + 24, static_cast<int>(panelY(settings, 24)), 18, Color{47, 125, 96, 255});
    DrawText("Raylib Simulation", left + 24, static_cast<int>(panelY(settings, 52)), 28, Color{28, 35, 32, 255});
    DrawText(settings.running ? "Running" : "Paused",
             left + 24,
             static_cast<int>(panelY(settings, 88)),
             18,
             settings.running ? Color{47, 125, 96, 255} : Color{185, 67, 76, 255});

    drawLabel(left + 24, static_cast<int>(panelY(settings, 128)), TextFormat("Environment: %s", environmentName(settings.environment)));
    drawLabel(left + 24, static_cast<int>(panelY(settings, 194)), settings.environment == EnvironmentMode::Maze
        ? "Maze: 4 robots advance as a team"
        : "Sandbox: click the field to set target");

    drawStatCard(left + 24, static_cast<int>(panelY(settings, 244)), "Robots", TextFormat("%d", static_cast<int>(world.robots().size())));
    drawStatCard(left + 180, static_cast<int>(panelY(settings, 244)), "Step", TextFormat("%d", settings.step));
    drawStatCard(left + 24, static_cast<int>(panelY(settings, 314)), "Avg Target", TextFormat("%.1f", stats.averageDistance));
    drawStatCard(left + 180, static_cast<int>(panelY(settings, 314)), "Closest", TextFormat("%.1f", stats.nearestPair));
    drawStatCard(left + 24, static_cast<int>(panelY(settings, 384)), "Avg Speed", TextFormat("%.1f", stats.averageSpeed));
    drawStatCard(left + 180, static_cast<int>(panelY(settings, 384)), settings.environment == EnvironmentMode::Maze ? "Checkpoint" : "Obstacles",
                 settings.environment == EnvironmentMode::Maze ? TextFormat("%d/10", std::min(settings.mazeCheckpoint + 1, 10)) : TextFormat("%d", static_cast<int>(world.obstacles().size())));

    drawLabel(left + 24, static_cast<int>(panelY(settings, 476)), TextFormat("Speed %.2fx", settings.speed));
    drawLabel(left + 180, static_cast<int>(panelY(settings, 476)), TextFormat("Energy %.2fx", settings.swarmEnergy));
    drawLabel(left + 24, static_cast<int>(panelY(settings, 562)), settings.environment == EnvironmentMode::Maze ? "Robots 4 fixed" : TextFormat("Robots %d", settings.robotCount));
    drawLabel(left + 180, static_cast<int>(panelY(settings, 562)), settings.environment == EnvironmentMode::Maze ? "Maze walls fixed" : TextFormat("Rocks %d", settings.rockCount));
    drawLabel(left + 24, static_cast<int>(panelY(settings, 648)), settings.environment == EnvironmentMode::Maze ? "Group checkpoints" : TextFormat("Walls %d", settings.wallCount));
    drawLabel(left + 180, static_cast<int>(panelY(settings, 648)), settings.environment == EnvironmentMode::Maze ? "Target Auto" : TextFormat("Target %s", settings.clickTarget ? "Click" : "Fixed"));

    DrawText("Controls", left + 24, static_cast<int>(panelY(settings, 770)), 20, Color{28, 35, 32, 255});
    DrawText("Space pause  N step  R reset", left + 24, static_cast<int>(panelY(settings, 802)), 16, Color{92, 105, 98, 255});
    DrawText("- / = speed  M click target", left + 24, static_cast<int>(panelY(settings, 828)), 16, Color{92, 105, 98, 255});
    DrawText("Scroll this panel for all options.", left + 24, static_cast<int>(panelY(settings, 854)), 16, Color{92, 105, 98, 255});

    EndScissorMode();
    drawScrollbar(settings);
}

void applySwarmEnergy(World& world, const Settings& settings) {
    for (Robot& robot : world.robots()) {
        robot.setMaxSpeed(8.4 * settings.swarmEnergy);
        robot.setMaxForce(9.5 * settings.swarmEnergy);
    }
}

void seedSandboxWorld(World& world, std::mt19937& rng, const Settings& settings) {
    std::uniform_real_distribution<double> xDistribution(3.0, world.width() - 4.0);
    std::uniform_real_distribution<double> yDistribution(3.0, world.height() - 4.0);
    std::uniform_real_distribution<double> velocityDistribution(-1.0, 1.0);

    world.setTarget({world.width() * 0.72, world.height() * 0.48});

    const std::vector<Vector2D> rockPositions{
        {world.width() * 0.30, world.height() * 0.32},
        {world.width() * 0.47, world.height() * 0.66},
        {world.width() * 0.62, world.height() * 0.28},
        {world.width() * 0.24, world.height() * 0.72},
        {world.width() * 0.78, world.height() * 0.36},
        {world.width() * 0.56, world.height() * 0.46},
        {world.width() * 0.38, world.height() * 0.52},
        {world.width() * 0.70, world.height() * 0.78}
    };

    for (int i = 0; i < settings.rockCount; ++i) {
        const Vector2D base = i < static_cast<int>(rockPositions.size())
            ? rockPositions[static_cast<size_t>(i)]
            : Vector2D{xDistribution(rng), yDistribution(rng)};
        world.addRock(base, 1.6 + static_cast<double>(i % 3) * 0.35);
    }

    const std::vector<std::pair<Vector2D, Vector2D>> walls{
        {{world.width() * 0.15, world.height() * 0.54}, {14.0, 1.45}},
        {{world.width() * 0.72, world.height() * 0.50}, {1.55, 13.0}},
        {{world.width() * 0.43, world.height() * 0.18}, {16.0, 1.35}},
        {{world.width() * 0.11, world.height() * 0.18}, {1.45, 10.0}},
        {{world.width() * 0.57, world.height() * 0.82}, {17.0, 1.35}}
    };

    for (int i = 0; i < settings.wallCount; ++i) {
        const auto& wall = walls[static_cast<size_t>(i % walls.size())];
        world.addWall(wall.first, wall.second);
    }

    for (int i = 0; i < settings.robotCount; ++i) {
        world.addRobot(
            std::make_unique<PathfindingBehavior>(),
            {xDistribution(rng), yDistribution(rng)},
            {velocityDistribution(rng), velocityDistribution(rng)},
            'P');
    }

    applySwarmEnergy(world, settings);
}

void seedMazeWorld(World& world, const Settings& settings) {
    world.setTarget(mazeCheckpoints(world)[0]);

    world.addWall({0.0, 0.0}, {world.width(), 1.0});
    world.addWall({0.0, world.height() - 1.0}, {world.width(), 1.0});
    world.addWall({0.0, 0.0}, {1.0, world.height()});
    world.addWall({world.width() - 1.0, 0.0}, {1.0, world.height()});

    world.addWall({10.0, 8.0}, {1.3, 36.0});
    world.addWall({20.0, 1.0}, {1.3, 35.0});
    world.addWall({30.0, 14.0}, {1.3, 35.0});
    world.addWall({40.0, 1.0}, {1.3, 35.0});
    world.addWall({50.0, 14.0}, {1.3, 35.0});
    world.addWall({60.0, 1.0}, {1.3, 35.0});
    world.addWall({70.0, 14.0}, {1.3, 35.0});

    world.addWall({10.0, 8.0}, {10.0, 1.2});
    world.addWall({20.0, 35.0}, {10.0, 1.2});
    world.addWall({30.0, 14.0}, {10.0, 1.2});
    world.addWall({40.0, 35.0}, {10.0, 1.2});
    world.addWall({50.0, 14.0}, {10.0, 1.2});
    world.addWall({60.0, 35.0}, {10.0, 1.2});

    const std::vector<Vector2D> starts{
        {4.2, world.height() - 5.8},
        {5.4, world.height() - 4.6},
        {6.6, world.height() - 5.8},
        {5.4, world.height() - 7.0}
    };

    for (const Vector2D& start : starts) {
        Robot& robot = world.addRobot(std::make_unique<PathfindingBehavior>(), start, {}, 'P');
        robot.setMaxSpeed(7.3 * settings.swarmEnergy);
        robot.setMaxForce(9.5 * settings.swarmEnergy);
    }
}

void seedInteractiveWorld(World& world, std::mt19937& rng, Settings& settings) {
    settings.mazeCheckpoint = 0;

    if (settings.environment == EnvironmentMode::Maze) {
        seedMazeWorld(world, settings);
    } else {
        seedSandboxWorld(world, rng, settings);
    }
}

void updateMazeProgress(World& world, Settings& settings) {
    if (settings.environment != EnvironmentMode::Maze) {
        return;
    }

    const std::vector<Vector2D> checkpoints = mazeCheckpoints(world);
    if (settings.mazeCheckpoint >= static_cast<int>(checkpoints.size())) {
        return;
    }

    int arrived = 0;
    const Vector2D checkpoint = checkpoints[static_cast<size_t>(settings.mazeCheckpoint)];
    for (const Robot& robot : world.robots()) {
        if (robot.position().distanceTo(checkpoint) < 3.2) {
            ++arrived;
        }
    }

    if (arrived >= 3 && settings.mazeCheckpoint + 1 < static_cast<int>(checkpoints.size())) {
        ++settings.mazeCheckpoint;
        world.setTarget(checkpoints[static_cast<size_t>(settings.mazeCheckpoint)]);
    }
}

void updatePanelScroll(Settings& settings) {
    const Vector2 mouse = GetMousePosition();
    const float maxScroll = std::max(0.0f, static_cast<float>(panelContentHeight - GetScreenHeight()));

    if (mouse.x >= currentWorldWidth()) {
        settings.panelScroll -= GetMouseWheelMove() * 44.0f;
    }

    settings.panelScroll = std::clamp(settings.panelScroll, 0.0f, maxScroll);
}
}

int runRaylibSimulation(double initialSpeed) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(initialScreenWidth, initialScreenHeight, "Swarm Robots Simulation - raylib");
    SetWindowMinSize(minimumScreenWidth, minimumScreenHeight);
    SetTargetFPS(60);

    std::mt19937 rng(1337);
    Settings settings;
    settings.speed = std::clamp(initialSpeed, 0.25, 5.0);
    World world(logicalWorldWidth, logicalWorldHeight);
    seedInteractiveWorld(world, rng, settings);

    auto reset = [&]() {
        world = World(logicalWorldWidth, logicalWorldHeight);
        seedInteractiveWorld(world, rng, settings);
        settings.step = 0;
    };

    while (!WindowShouldClose()) {
        updatePanelScroll(settings);

        if (IsKeyPressed(KEY_SPACE)) {
            settings.running = !settings.running;
        }
        if (IsKeyPressed(KEY_R)) {
            reset();
        }
        if (IsKeyPressed(KEY_M)) {
            settings.clickTarget = !settings.clickTarget;
        }
        if (IsKeyPressed(KEY_MINUS)) {
            settings.speed = std::max(0.25, settings.speed - 0.25);
        }
        if (IsKeyPressed(KEY_EQUAL)) {
            settings.speed = std::min(5.0, settings.speed + 0.25);
        }

        const Vector2 mouse = GetMousePosition();
        if (settings.environment == EnvironmentMode::Sandbox &&
            settings.clickTarget &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            mouse.x >= 0 && mouse.x < currentWorldWidth() &&
            mouse.y >= 0 && mouse.y < currentWorldHeight()) {
            world.setTarget(toWorldPoint(world, mouse));
        }

        const bool stepOnce = IsKeyPressed(KEY_N);
        if (settings.running || stepOnce) {
            world.update(GetFrameTime() * settings.speed);
            updateMazeProgress(world, settings);
            ++settings.step;
        }

        BeginDrawing();
        drawGrid();
        drawMazeRoute(world, settings);
        drawObstacles(world);
        drawTarget(world);
        for (const Robot& robot : world.robots()) {
            drawRobot(world, robot);
        }

        drawPanelBackground();
        drawPanelContent(world, settings);

        const int left = currentWorldWidth();
        const Button sandboxButton{{left + 24.0f, panelY(settings, 154.0f), 136.0f, 40.0f}, "Sandbox", settings.environment == EnvironmentMode::Sandbox};
        const Button mazeButton{{left + 180.0f, panelY(settings, 154.0f), 136.0f, 40.0f}, "Maze", settings.environment == EnvironmentMode::Maze};
        const Button speedDown{{left + 24.0f, panelY(settings, 502.0f), 64.0f, 38.0f}, "-"};
        const Button speedUp{{left + 96.0f, panelY(settings, 502.0f), 64.0f, 38.0f}, "+"};
        const Button energyDown{{left + 180.0f, panelY(settings, 502.0f), 64.0f, 38.0f}, "-"};
        const Button energyUp{{left + 252.0f, panelY(settings, 502.0f), 64.0f, 38.0f}, "+"};
        const Button robotsDown{{left + 24.0f, panelY(settings, 588.0f), 64.0f, 38.0f}, "-"};
        const Button robotsUp{{left + 96.0f, panelY(settings, 588.0f), 64.0f, 38.0f}, "+"};
        const Button rocksDown{{left + 180.0f, panelY(settings, 588.0f), 64.0f, 38.0f}, "-"};
        const Button rocksUp{{left + 252.0f, panelY(settings, 588.0f), 64.0f, 38.0f}, "+"};
        const Button wallsDown{{left + 24.0f, panelY(settings, 674.0f), 64.0f, 38.0f}, "-"};
        const Button wallsUp{{left + 96.0f, panelY(settings, 674.0f), 64.0f, 38.0f}, "+"};
        const Button targetButton{{left + 180.0f, panelY(settings, 674.0f), 136.0f, 38.0f}, settings.environment == EnvironmentMode::Maze ? "Auto Target" : (settings.clickTarget ? "Click On" : "Click Off")};
        const Button pauseButton{{left + 24.0f, panelY(settings, 720.0f), 136.0f, 40.0f}, settings.running ? "Pause" : "Start"};
        const Button resetButton{{left + 180.0f, panelY(settings, 720.0f), 136.0f, 40.0f}, "Reset"};

        if (drawButton(sandboxButton) && settings.environment != EnvironmentMode::Sandbox) {
            settings.environment = EnvironmentMode::Sandbox;
            reset();
        }
        if (drawButton(mazeButton) && settings.environment != EnvironmentMode::Maze) {
            settings.environment = EnvironmentMode::Maze;
            reset();
        }
        if (drawButton(speedDown)) settings.speed = std::max(0.25, settings.speed - 0.25);
        if (drawButton(speedUp)) settings.speed = std::min(5.0, settings.speed + 0.25);
        if (drawButton(energyDown)) {
            settings.swarmEnergy = std::max(0.5, settings.swarmEnergy - 0.15);
            applySwarmEnergy(world, settings);
        }
        if (drawButton(energyUp)) {
            settings.swarmEnergy = std::min(3.0, settings.swarmEnergy + 0.15);
            applySwarmEnergy(world, settings);
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(robotsDown)) {
            settings.robotCount = std::max(10, settings.robotCount - 4);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(robotsUp)) {
            settings.robotCount = std::min(90, settings.robotCount + 4);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(rocksDown)) {
            settings.rockCount = std::max(0, settings.rockCount - 1);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(rocksUp)) {
            settings.rockCount = std::min(12, settings.rockCount + 1);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(wallsDown)) {
            settings.wallCount = std::max(0, settings.wallCount - 1);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(wallsUp)) {
            settings.wallCount = std::min(8, settings.wallCount + 1);
            reset();
        }
        if (settings.environment == EnvironmentMode::Sandbox && drawButton(targetButton)) settings.clickTarget = !settings.clickTarget;
        if (drawButton(pauseButton)) settings.running = !settings.running;
        if (drawButton(resetButton)) reset();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
