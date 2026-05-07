/*
** EPITECH PROJECT, 2026
** main
** File description:
** Entry point — parse args, load scene, render to stdout
*/

#include <iostream>
#include <chrono>
#include <iomanip>
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
        auto t0 = std::chrono::steady_clock::now();
        RayTracer::ConfigParser cfgParser;
        RayTracer::Scene scene = cfgParser.parse(argv[1]);
        RayTracer::Renderer renderer;

        auto t1 = std::chrono::steady_clock::now();
        renderer.render(scene, std::cout);
        auto t2 = std::chrono::steady_clock::now();

        double loadTime   = std::chrono::duration<double>(t1 - t0).count();
        double renderTime = std::chrono::duration<double>(t2 - t1).count();
        double totalTime  = std::chrono::duration<double>(t2 - t0).count();
        std::cerr << std::fixed << std::setprecision(2)
                  << "Load:   " << loadTime   << "s\n"
                  << "Render: " << renderTime << "s\n"
                  << "Total:  " << totalTime  << "s\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
    return 0;
}
