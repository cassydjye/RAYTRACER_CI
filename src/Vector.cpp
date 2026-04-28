/*
** EPITECH PROJECT, 2026
** vector
** File description:
** vector
*/

#include "../include/Vector.hpp"

double Math::Vector3D::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

double Math::Vector3D::dot(const Vector3D& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Math::Vector3D Math::Vector3D::operator+(const Vector3D& other) const
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Math::Vector3D Math::Vector3D::operator-(const Vector3D& other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Math::Vector3D Math::Vector3D::operator*(double scalar) const
{
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Math::Vector3D Math::Vector3D::operator/(double scalar) const
{
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

Math::Vector3D Math::Vector3D::normalize() const
{
    double len = length();
    return Vector3D(x / len, y / len, z / len);
}
