#pragma once

#include <iosfwd>

class World;

class Renderer {
public:
    Renderer(int width, int height);

    void draw(const World& world, int step, std::ostream& out) const;

private:
    int width_{80};
    int height_{25};
};
