/*
** EPITECH PROJECT, 2024
** rts
** File description:
** using
*/

#pragma once
#include <bitset>
#include <cstdint>
#include <queue>
#include <array>
#include <unordered_map>
#include <memory>
#include <set>
#include <chrono>
#include <algorithm>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 500;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

enum bitToRole {
    PLAYER,
    BULLET,
    WALL,
    MONSTER,
};

void manageTime(std::time_t &start_time, int &second)
{
    auto current = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current);
    if (current_time != start_time)
    {
        second += 1;
        start_time = current_time;
    }
}