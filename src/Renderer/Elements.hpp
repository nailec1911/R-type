/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Elements
*/

#pragma once

#include <map>
#include <string>
#include <vector>

enum elementTypes {
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4,
    PLAYER5,
    BULLET_P,
    BULLET_P_FIRE,
    BULLET_P_CHARGED,
};

/**
 * @brief Vector for 2 Dimensional reference
 * @tparam T Type of the vector
 */
template<typename T>
struct Vector2 {
    T x; ///< x value
    T y; ///< y value
};

struct elementInfo {
    std::string filepath;
    std::vector<Vector2<Vector2<float>>> frames;
    int sizeX;
    int sizeY;
};


const std::map<elementTypes, elementInfo> eltInfo {
    {PLAYER1, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 3}, {98,14}}}, 31, 11}},
    {BULLET_P_FIRE, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{233,85}, {243,95}}, {{215, 85}, {228,96}}, {{249, 90}, {264,93}}}, 18, 10}}
};
