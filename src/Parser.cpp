/*
** EPITECH PROJECT, 2026
** Parser
** File description:
** Parser
*/

#include "../include/Parser.hpp"

Parser::Parser(int ac, char **av) : _displayHelp(false)
{
    parseArguments(ac, av);
}

bool Parser::shouldDisplayHelp() const
{
    return _displayHelp;
}

void Parser::parseArguments(int ac, char **av)
{
    if (ac != 2)
        return;
    if (std::string(av[1]) == "-h" || std::string(av[1]) == "--help") {
        _displayHelp = true;
        return;
    }
}
