/*
** EPITECH PROJECT, 2026
** camera
** File description:
** camera
*/

#include "../include/Camera.hpp"

RayTracer::Ray RayTracer::Camera::ray(double u, double v) const
{
    return Ray(origin, screen.pointAt(u, v) - origin);
}