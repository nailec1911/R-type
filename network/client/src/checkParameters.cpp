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
#include <vector>
#include <stdbool.h>

static
bool checkIp(const std::vector<int> &ipTable) 
{
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

bool checkParameters(int argc, char **argv)
{
    if (argc != 3)
        return false;
    if (!checkIp(split(argv[1], '.')))
        return false;
    if (std::stoi(argv[2]) > 65535 || std::stoi(argv[2]) < 1024)
        return false;
    return true;
}