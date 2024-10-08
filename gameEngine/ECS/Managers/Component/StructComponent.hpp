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

struct BulletPlayer
{};

struct BulletMonster
{};

struct Monster
{};

struct ShooterMonster
{};

struct FlyingMonster
{};

struct Wall
{};

struct HUDComp
{
    std::string body;
};
