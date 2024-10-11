/*
** EPITECH PROJECT, 2024
** rts
** File description:
** check_parameters
*/
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cstring>
#include <stdbool.h>
#include "../../Errors.hpp"

static
bool checkIp(const std::vector<int> &ipTable) 
{
    if (ipTable.size() != 4)
        return false;
    return std::all_of(ipTable.begin(), ipTable.end(), [](int elem) {
        return elem >= 0 && elem <= 255;
    });
}

static 
std::vector<int> split(const std::string &str, char delimiter)
{
    std::vector<int> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

int checkParametersClient(int argc, char **argv)
{
    if ((argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) || argc != 3) {
        throw HelpExceptionClient();
        return 0;
    }
    if (!checkIp(split(argv[1], '.'))) {
        throw ErrorParams(ERROR_IP);
        return 0;
    }
    if (std::stoi(argv[2]) > 65535 || std::stoi(argv[2]) < 1024) {
        throw ErrorParams(ERROR_PORT);
        return 0;
    }
    return 0;
}