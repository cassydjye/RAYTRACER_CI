/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** ObjLoader
*/

#include "../../include/scene/ObjLoader.hpp"
#include "../../include/primitives/Triangles.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>

struct FaceVertex {
    int v  = -1; // vertex index
    int vn = -1; // normal index (-1 = none)
};

// Parse a face token: "v", "v/vt", "v//vn", "v/vt/vn"
static FaceVertex parseFaceToken(const std::string& token)
{
    FaceVertex fv;
    size_t first = token.find('/');
    fv.v = std::stoi(token.substr(0, first)) - 1;
    if (first == std::string::npos) return fv;
    size_t second = token.find('/', first + 1);
    if (second != std::string::npos && second > first + 1)
        fv.vn = std::stoi(token.substr(second + 1)) - 1;
    else if (second == std::string::npos && first + 1 < token.size())
        fv.vn = std::stoi(token.substr(first + 1)) - 1;
    return fv;
}

RayTracer::Mesh RayTracer::ObjLoader::load(const std::string& path,
                                            const Math::Point3D& position,
                                            std::shared_ptr<IMaterial> material)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("ObjLoader: cannot open file: " + path);

    std::vector<Math::Point3D>  vertices;
    std::vector<Math::Vector3D> vnormals;

    // Raw face data: each face = list of FaceVertex
    struct Face { std::vector<FaceVertex> fvs; };
    std::vector<Face> faces;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            double x, y, z; ss >> x >> y >> z;
            vertices.emplace_back(x + position.x, y + position.y, z + position.z);
        } else if (token == "vn") {
            double x, y, z; ss >> x >> y >> z;
            vnormals.emplace_back(x, y, z);
        } else if (token == "f") {
            Face face;
            std::string t;
            while (ss >> t)
                face.fvs.push_back(parseFaceToken(t));
            if (face.fvs.size() >= 3)
                faces.push_back(std::move(face));
        }
    }

    if (vertices.empty())
        throw std::runtime_error("ObjLoader: no vertices found in " + path);

    // Always compute smooth normals as a fallback for faces whose file normals
    // are missing or out-of-range, so the else branch below is always safe.
    bool useFileNormals = !vnormals.empty();
    std::vector<Math::Vector3D> smoothNormals(vertices.size(), Math::Vector3D(0, 0, 0));
    {
        auto svValid = [&](int i) { return i >= 0 && static_cast<size_t>(i) < vertices.size(); };
        for (const auto& face : faces) {
            for (size_t i = 1; i + 1 < face.fvs.size(); ++i) {
                int a = face.fvs[0].v, b = face.fvs[i].v, c = face.fvs[i + 1].v;
                if (!svValid(a) || !svValid(b) || !svValid(c)) continue;
                Math::Vector3D e1 = vertices[b] - vertices[a];
                Math::Vector3D e2 = vertices[c] - vertices[a];
                Math::Vector3D geom(e1.y*e2.z - e1.z*e2.y,
                                    e1.z*e2.x - e1.x*e2.z,
                                    e1.x*e2.y - e1.y*e2.x);
                smoothNormals[a] = smoothNormals[a] + geom;
                smoothNormals[b] = smoothNormals[b] + geom;
                smoothNormals[c] = smoothNormals[c] + geom;
            }
        }
        for (auto& n : smoothNormals) {
            double len = n.length();
            if (len > 1e-8) n = n / len;
        }
    }

    // Build triangles.
    auto vValid  = [&](int i) { return i >= 0 && static_cast<size_t>(i) < vertices.size(); };
    auto vnValid = [&](int i) { return i >= 0 && static_cast<size_t>(i) < vnormals.size(); };

    std::vector<Triangles> triangles;
    for (const auto& face : faces) {
        for (size_t i = 1; i + 1 < face.fvs.size(); ++i) {
            const FaceVertex& fv0 = face.fvs[0];
            const FaceVertex& fv1 = face.fvs[i];
            const FaceVertex& fv2 = face.fvs[i + 1];

            if (!vValid(fv0.v) || !vValid(fv1.v) || !vValid(fv2.v))
                continue;

            if (useFileNormals && vnValid(fv0.vn) && vnValid(fv1.vn) && vnValid(fv2.vn)) {
                triangles.emplace_back(
                    vertices[fv0.v], vertices[fv1.v], vertices[fv2.v],
                    vnormals[fv0.vn], vnormals[fv1.vn], vnormals[fv2.vn],
                    material
                );
            } else {
                triangles.emplace_back(
                    vertices[fv0.v], vertices[fv1.v], vertices[fv2.v],
                    smoothNormals[fv0.v], smoothNormals[fv1.v], smoothNormals[fv2.v],
                    material
                );
            }
        }
    }

    if (triangles.empty())
        throw std::runtime_error("ObjLoader: no triangles found in " + path);

    return Mesh(std::move(triangles));
}
