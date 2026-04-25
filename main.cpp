/*
** EPITECH PROJECT, 2026
** main
** File description:
** main
*/

#include "include/Parser.hpp"
#include "include/Help.hpp"

int main(int ac, char **av)
{
    try {
        Parser parser(ac, av);
        if (parser.shouldDisplayHelp())
            return Help::printHelp();
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 84;
}