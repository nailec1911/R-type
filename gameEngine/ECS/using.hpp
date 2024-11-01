/*
** EPITECH PROJECT, 2024
** rts
** File description:
** using
*/

#pragma once
#include <bitset>
#include <chrono>
#include <cstdint>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 500;
const Entity ENTITY_ERROR = UINT32_MAX;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
using chrono = std::chrono::steady_clock;

enum EntityName : uint8_t
{
    NONE,
    DESTROY,
    PLAYER,
    CHRONO,
    P_BULLET,
    WALL,
    MONSTER,
    HUD,
    P_BULLET_CHARGED,
    M_BULLET,
    BONUS_SHOOT,
    BONUS_SPEED,
    TRANSFORM,
    POSITION,
    BOUNDING_BOX,
    SHOOTER_MONSTER,
    FLYING_MONSTER,
};

inline std::chrono::steady_clock::duration initTickRate(int tickRate)
{
    auto tickDuration = std::chrono::duration_cast<chrono::duration>(
        std::chrono::duration<double>(1.0 / tickRate));
    return tickDuration;
}
