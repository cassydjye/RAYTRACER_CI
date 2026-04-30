/*
** EPITECH PROJECT, 2026
** main
** File description:
** Entry point — parse args, load scene, render to stdout
*/

#include <iostream>
#include "include/scene/ConfigParser.hpp"
#include "include/core/Renderer.hpp"
#include "include/Parser.hpp"
#include "include/Help.hpp"

int main(int argc, char* argv[])
{
    Parser parser(argc, argv);
    if (parser.shouldDisplayHelp())
        return Help::printHelp();
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
