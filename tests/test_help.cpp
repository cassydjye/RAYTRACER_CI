#include <criterion/criterion.h>
#include <iostream>
#include <sstream>
#include <string>
#include "../include/Help.hpp"

Test(help, print_help_output)
{
    std::streambuf *old = std::cout.rdbuf();
    std::ostringstream capture;
    std::cout.rdbuf(capture.rdbuf());

    Help::printHelp();

    std::cout.rdbuf(old);

    std::string output = capture.str();

    std::string expected =
        "USAGE: ./raytracer <SCENE_FILE>\n"
        "   SCENE_FILE: scene configuration\n";

    cr_assert_str_eq(output.c_str(), expected.c_str());
}