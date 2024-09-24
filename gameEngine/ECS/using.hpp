/*
** EPITECH PROJECT, 2024
** rts
** File description:
** using
*/

#pragma once
#include <bitset>
#include <cstdint>

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
