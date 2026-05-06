/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Transform
*/

#include "../../include/primitives/Transform.hpp"
#include "../../include/raytracer/Ray.hpp"
#include "../../include/raytracer/HitRecord.hpp"
#include "../../include/raytracer/AABB.hpp"

#include <cmath>

namespace RayTracer {

// Build R = Rz * Ry * Rx  (XYZ extrinsic order, same convention as the original rotatePoint helper).
Transform::Transform(std::unique_ptr<IPrimitive> inner,
                     const Math::Point3D& pivot,
                     double rx, double ry, double rz)
    : _inner(std::move(inner)), _pivot(pivot)
{
    double cx = std::cos(rx), sx = std::sin(rx);
    double cy = std::cos(ry), sy = std::sin(ry);
    double cz = std::cos(rz), sz = std::sin(rz);

    // Rx
    // [1   0   0 ]
    // [0   cx -sx]
    // [0   sx  cx]

    // Ry
    // [ cy  0  sy]
    // [  0  1   0]
    // [-sy  0  cy]

    // Rz
    // [cz -sz  0]
    // [sz  cz  0]
    // [ 0   0  1]

    // R = Rz * Ry * Rx  (column-major application order matches rotatePoint)
    _R[0] =  cy*cz;              _R[1] = cz*sx*sy - cx*sz;    _R[2] = cx*cz*sy + sx*sz;
    _R[3] =  cy*sz;              _R[4] = cx*cz + sx*sy*sz;    _R[5] = cx*sy*sz - cz*sx;
    _R[6] = -sy;                 _R[7] = cy*sx;               _R[8] = cx*cy;

    // R^T (transpose = inverse for orthogonal matrix)
    _Rt[0] = _R[0]; _Rt[1] = _R[3]; _Rt[2] = _R[6];
    _Rt[3] = _R[1]; _Rt[4] = _R[4]; _Rt[5] = _R[7];
    _Rt[6] = _R[2]; _Rt[7] = _R[5]; _Rt[8] = _R[8];
}

Math::Vector3D Transform::applyMatrix(const std::array<double, 9>& m, const Math::Vector3D& v)
{
    return Math::Vector3D(
        m[0]*v.x + m[1]*v.y + m[2]*v.z,
        m[3]*v.x + m[4]*v.y + m[5]*v.z,
        m[6]*v.x + m[7]*v.y + m[8]*v.z
    );
}

bool Transform::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    // Transform ray to object space (apply R^T).
    Math::Vector3D originOffset(
        ray.origin.x - _pivot.x,
        ray.origin.y - _pivot.y,
        ray.origin.z - _pivot.z
    );
    Math::Vector3D objOriginVec = applyMatrix(_Rt, originOffset);
    Math::Point3D  objOrigin(_pivot.x + objOriginVec.x,
                              _pivot.y + objOriginVec.y,
                              _pivot.z + objOriginVec.z);
    Math::Vector3D objDir = applyMatrix(_Rt, ray.direction);

    Ray objRay(objOrigin, objDir);

    HitRecord objRec;
    if (!_inner->hits(objRay, tMin, tMax, objRec))
        return false;

    // Transform hit results back to world space (apply R).
    Math::Vector3D hitOffset(
        objRec.point.x - _pivot.x,
        objRec.point.y - _pivot.y,
        objRec.point.z - _pivot.z
    );
    Math::Vector3D worldHitVec = applyMatrix(_R, hitOffset);
    rec.t        = objRec.t;
    rec.point    = Math::Point3D(_pivot.x + worldHitVec.x,
                                  _pivot.y + worldHitVec.y,
                                  _pivot.z + worldHitVec.z);
    rec.normal   = applyMatrix(_R, objRec.normal);
    rec.material = objRec.material;
    return true;
}

std::optional<AABB> Transform::boundingBox() const
{
    auto inner = _inner->boundingBox();
    if (!inner.has_value()) return std::nullopt;

    const AABB& b = inner.value();
    double xs[2] = {b.min.x, b.max.x};
    double ys[2] = {b.min.y, b.max.y};
    double zs[2] = {b.min.z, b.max.z};

    double mnx =  1e18, mny =  1e18, mnz =  1e18;
    double mxx = -1e18, mxy = -1e18, mxz = -1e18;

    for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
    for (int k = 0; k < 2; ++k) {
        Math::Vector3D corner(xs[i] - _pivot.x, ys[j] - _pivot.y, zs[k] - _pivot.z);
        Math::Vector3D w = applyMatrix(_R, corner);
        double wx = _pivot.x + w.x, wy = _pivot.y + w.y, wz = _pivot.z + w.z;
        if (wx < mnx) mnx = wx;
        if (wx > mxx) mxx = wx;
        if (wy < mny) mny = wy;
        if (wy > mxy) mxy = wy;
        if (wz < mnz) mnz = wz;
        if (wz > mxz) mxz = wz;
    }

    return AABB(Math::Point3D(mnx, mny, mnz), Math::Point3D(mxx, mxy, mxz));
}

} // namespace RayTracer
