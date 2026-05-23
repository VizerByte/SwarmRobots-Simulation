#pragma once

#include <algorithm>
#include <cmath>
#include <ostream>

struct Vector2D {
    double x{0.0};
    double y{0.0};

    Vector2D() = default;
    Vector2D(double xValue, double yValue) : x(xValue), y(yValue) {}

    double lengthSquared() const {
        return x * x + y * y;
    }

    double length() const {
        return std::sqrt(lengthSquared());
    }

    Vector2D normalized() const {
        const double len = length();
        if (len <= 1e-9) {
            return {};
        }
        return {x / len, y / len};
    }

    Vector2D limited(double maxLength) const {
        const double len = length();
        if (len <= maxLength || len <= 1e-9) {
            return *this;
        }
        return normalized() * maxLength;
    }

    double distanceTo(const Vector2D& other) const {
        return (*this - other).length();
    }

    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2D operator-(const Vector2D& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2D operator/(double scalar) const {
        return {x / scalar, y / scalar};
    }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

inline Vector2D operator*(double scalar, const Vector2D& vector) {
    return vector * scalar;
}

inline std::ostream& operator<<(std::ostream& out, const Vector2D& vector) {
    out << "(" << vector.x << ", " << vector.y << ")";
    return out;
}
