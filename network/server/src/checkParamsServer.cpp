/*
** EPITECH PROJECT, 2024
** rts
** File description:
** checkParamsServer
*/
#include <stdbool.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

bool checkParamsServer(int argc, char **argv)
{
    if (argc != 2)
        return false;
    if (std::stoi(argv[1]) < 1024 || std::stoi(argv[1]) > 65535)
        return false;
    return true;
}