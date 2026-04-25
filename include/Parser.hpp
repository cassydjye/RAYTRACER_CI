/*
** EPITECH PROJECT, 2026
** Parser
** File description:
** Parser
*/

#pragma once

#include <string>

class Parser {
    private:
        bool _displayHelp;
        void parseArguments(int ac, char **av);

    public:
        Parser(int ac, char **av);
        ~Parser() = default;
        bool shouldDisplayHelp() const;
};
