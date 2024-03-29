#include "vector2.hpp"

#include <cmath>

namespace common {

Vector2::Vector2(double x, double y) : x(x), y(y) {}

Vector2 Vector2::polar(double angle, double magnitude) {
    double x = magnitude * std::cos(angle);
    double y = magnitude * std::sin(angle);

    return Vector2(x, y);
}

double Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

double Vector2::angle() const {
    return std::atan2(y, x);
}

double Vector2::dot(const Vector2 rhs) const {
    return x * rhs.x + y * rhs.y;
}

Vector2& Vector2::operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2& Vector2::operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator*(const Vector2& lhs, double scalar) {
    return Vector2(lhs.x * scalar, lhs.y * scalar);
}

Vector2 operator*(double scalar, const Vector2& rhs) {
    return Vector2(rhs.x * scalar, rhs.y * scalar);
}

std::ostream& operator<<(std::ostream& output, const Vector2& vector) {
    return output << "<" << vector.x << ", " << vector.y << ">";
}

}  // namespace common

namespace nlohmann {

void adl_serializer<common::Vector2>::to_json(nlohmann::ordered_json& json, const common::Vector2& vector) {
    json = nlohmann::ordered_json::array({vector.x, vector.y});
}

common::Vector2 adl_serializer<common::Vector2>::from_json(const nlohmann::ordered_json& json) {
    double x = json.at(0).get<double>();
    double y = json.at(1).get<double>();

    return common::Vector2(x, y);
}

}  // namespace nlohmann
