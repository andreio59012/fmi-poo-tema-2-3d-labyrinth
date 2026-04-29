#pragma once
#include <cmath>

#define PI 3.14159265f
#define EPSILON 1e-5f

struct vec2i;

struct vec2 {
    float x, y;

    explicit vec2(const float x_ = 0.0f, const float y_ = 0.0f) : x(x_), y(y_) {}
    explicit vec2(const vec2i& src);

    vec2& operator+=(const vec2& a) { x += a.x; y += a.y; return *this; }
    vec2& operator-=(const vec2& a) { x -= a.x; y -= a.y; return *this; }
    vec2& operator*=(const vec2& a) { x *= a.x; y *= a.y; return *this; }
    vec2& operator*=(const float a) { x *= a; y *= a; return *this; }
    vec2& operator/=(const vec2& a) { x /= a.x; y /= a.y; return *this; }
    vec2& operator/=(const float a) { x /= a; y /= a; return *this; }

    [[nodiscard]] float get_length() const { return std::sqrt(x * x + y * y); }
    [[nodiscard]] float get_dist_to(const vec2& vec) const {
        const float dx = vec.x - x, dy = vec.y - y;
        return sqrtf(dx * dx + dy * dy);
    }
};

struct vec2i {
    int x, y;

    explicit vec2i(const int x_ = 0, const int y_ = 0) : x(x_), y(y_) {}
    explicit vec2i(const vec2& src);

    vec2i& operator+=(const vec2i& a) { x += a.x; y += a.y; return *this; }
    vec2i& operator-=(const vec2i& a) { x -= a.x; y -= a.y; return *this; }
    vec2i& operator*=(const float a) { x = static_cast<int>(x * a); y = static_cast<int>(y * a); return *this; }
    vec2i& operator*=(const vec2i& a) { x *= a.x; y *= a.y; return *this; }
    vec2i& operator/=(const float a) { x = static_cast<int>(x / a); y = static_cast<int>(y / a); return *this; }
    vec2i& operator/=(const vec2i& a) { x /= a.x; y /= a.y; return *this; }
};

inline vec2::vec2(const vec2i& src) : x(static_cast<float>(src.x)), y(static_cast<int>(src.y)) {}
inline vec2i::vec2i(const vec2& src) : x(static_cast<int>(src.x)), y(static_cast<int>(src.y)) {}

inline vec2 operator+(vec2 a, const vec2& b) { return a += b; }
inline vec2 operator-(vec2 a, const vec2& b) { return a -= b; }
inline vec2 operator*(vec2 a, const float b) { return a *= b; }
inline vec2 operator*(vec2 a, const vec2& b) { return a *= b; }
inline vec2 operator/(vec2 a, const float b) { return a /= b; }
inline vec2 operator/(vec2 a, const vec2& b) { return a /= b; }

inline vec2i operator+(vec2i a, const vec2i& b) { return a += b; }
inline vec2i operator-(vec2i a, const vec2i& b) { return a -= b; }
inline vec2i operator*(vec2i a, const float b) { return a *= b; }
inline vec2i operator*(vec2i a, const vec2i& b) { return a *= b; }
inline vec2i operator/(vec2i a, const float b) { return a /= b; }
inline vec2i operator/(vec2i a, const vec2i& b) { return a /= b; }

[[nodiscard]] inline int clamp(const int a, const int a_min, const int a_max) {
    if (a < a_min) return a_min;
    if (a > a_max) return a_max;
    return a;
}