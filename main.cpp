/*
** EPITECH PROJECT, 2026
** main
** File description:
** Entry point — parse args, load scene, render to stdout
*/

#include <iostream>
#include "src/scene/ConfigParser.hpp"
#include "src/core/Renderer.hpp"

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cerr << "USAGE: ./raytracer <SCENE_FILE>\n"
                  << "  SCENE_FILE: libconfig++ scene configuration\n";
        return 0;
    }
    if (argc != 2) {
        std::cerr << "USAGE: ./raytracer <SCENE_FILE>\n";
        return 84;
    }
    try {
        RayTracer::ConfigParser parser;
        RayTracer::Scene scene = parser.parse(argv[1]);
        RayTracer::Renderer renderer;
        renderer.render(scene, std::cout);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
    return 0;
}
