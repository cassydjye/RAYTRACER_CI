/*
** EPITECH PROJECT, 2026
** Parser
** File description:
** Parser
*/

#include "../include/Parser.hpp"
#include <stdexcept>

Parser::Parser(int ac, char **av) : _displayHelp(false)
{
    parseArguments(ac, av);
}

bool Parser::shouldDisplayHelp() const
{
    return _displayHelp;
}

void Parser::parseArguments(int ac, char **av)
{    if (ac == 2 && std::string(av[1]) == "--help") {
        _displayHelp = true;
    } else if (ac != 2) {
        printf("USAGE: ./raytracer <SCENE_FILE>\n");
    }
}
