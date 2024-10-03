/*
** EPITECH PROJECT, 2024
** rts
** File description:
** StructComponent
*/

#pragma once
#include <cstdint>
#include <string>

struct Position
{
    float x;
    float y;
};

struct BoundingBox
{
    float width;
    float height;
};

struct Transform
{
    float velX;
    float velY;
};

struct Player
{
    uint32_t id;
};

struct Bullet
{};

struct Monster
{};

struct Wall
{};

struct HUDComp
{
    std::string body;
};
