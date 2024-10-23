/*
** EPITECH PROJECT, 2024
** rts
** File description:
** checkParametersServer
*/

#include "../../Errors.hpp"

#include <cstring>

int checkParametersServer(int argc, char **argv)
{
    if ((argc == 2 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0))) {
        throw HelpExceptionServer();
        return 0;
    }
    if (argc == 2 && (std::stoi(argv[1]) > 65535 || std::stoi(argv[1]) < 1024)) {
        throw ErrorParams(ERROR_PORT);
        return 0;
    }
    if (argc != 2)
        throw HelpExceptionServer();
    return 0;
}
