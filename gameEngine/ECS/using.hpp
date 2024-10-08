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

enum EntityName {
    NONE,
    PLAYER,
    P_BULLET,
    WALL,
    MONSTER,
    HUD,
    P_BULLET_CHARGED,
    M_BULLET,
    TRANSFORM,
    POSITION,
    BOUNDING_BOX,
    SHOOTER_MONSTER,
    FLYING_MONSTER,
};
