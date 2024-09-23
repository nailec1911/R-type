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
    PLAYER1_UP,
    PLAYER1_DOWN,
    PLAYER2,
    PLAYER2_UP,
    PLAYER2_DOWN,
    PLAYER3,
    PLAYER3_UP,
    PLAYER3_DOWN,
    PLAYER4,
    PLAYER4_UP,
    PLAYER4_DOWN,
    PLAYER5,
    PLAYER5_UP,
    PLAYER5_DOWN,
    BULLET_P,
    BULLET_P_FIRE,
    BULLET_P_CHARGED_20,
    BULLET_P_CHARGED_40,
    BULLET_P_CHARGED_60,
    BULLET_P_CHARGED_80,
    BULLET_P_CHARGED_100,
    BULLET_M,
    CONCENTRATION_BULLET_CHARGED,
    JET_REACTOR_P,
    PLAYER_DEATH,
    MONSTER_DEATH,
    MONSTER_DEATH_2,
    RED_MONSTER_LEFT,
    RED_MONSTER_RIGHT,
    BROWN_MONSTER_LEFT,
    BROWN_MONSTER_RIGHT,
    GRAY_MONSTER_LEFT,
    GRAY_MONSTER_RIGHT,
    BONUS_LEVEL_1,
    BONUS_ITEM,
    WALL_1,
    WALL_2,
    WALL_3,
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
    {PLAYER1_UP, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 3}, {98,14}}, {{100,3}, {131,15}}, {{133,2}, {164, 15}}}, 31, 11}},
    {PLAYER1_DOWN, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 3}, {98,14}}, {{34,3}, {65,15}}, {{1,3}, {32, 16}}}, 31, 13}},

    {PLAYER2, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 20}, {98,31}}}, 31, 11}},
    {PLAYER2_UP, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 20}, {98,31}}, {{100,20}, {131,32}}, {{133,19}, {164, 32}}}, 31, 11}},
    {PLAYER2_DOWN, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 20}, {98,31}}, {{34,20}, {65,32}}, {{1,20}, {32, 33}}}, 31, 13}},

    {PLAYER3, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 37}, {98,48}}}, 31, 11}},
    {PLAYER3_UP, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 37}, {98,48}}, {{100,37}, {131,49}}, {{133,36}, {164, 49}}}, 31, 11}},
    {PLAYER3_DOWN, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 37}, {98,48}}, {{34,37}, {65,49}}, {{1,36}, {32, 49}}}, 31, 13}},

    {PLAYER4, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 54}, {98,66}}}, 31, 11}},
    {PLAYER4_UP, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 54}, {98,66}}, {{100,54}, {131,66}}, {{133,53}, {164, 66}}}, 31, 11}},
    {PLAYER4_DOWN, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 54}, {98,66}}, {{34,54}, {65,66}}, {{1,53}, {32, 66}}}, 31, 13}},

    {PLAYER5, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 71}, {98,83}}}, 31, 11}},
    {PLAYER5_UP, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 71}, {98,83}}, {{100,71}, {131,83}}, {{133,70}, {164, 84}}}, 31, 11}},
    {PLAYER5_DOWN, {"src/Renderer/assets/sprites/r-typesheet42.gif", {{{67, 71}, {98,83}}, {{34,71}, {65,83}}, {{1,71}, {32, 84}}}, 31, 11}},
    {PLAYER_DEATH, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{1, 343}, {32, 370}}, {{35, 343}, {66, 370}}, {{73, 348}, {93, 365}}, {{106, 347}, {127, 366}}, {{138, 346}, {161, 367}}, {{170, 345}, {195, 368}}, {{202, 344}, {229, 369}}, {{233, 343}, {264, 370}}}, 79, 13}},
    {JET_REACTOR_P, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{234, 26}, {264,41}}, {{200, 21}, {231, 45}}, {{167, 20}, {198,48}}, {{134, 18}, {165, 49}}}, 31, 31}},

    {BULLET_P_FIRE, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{233,85}, {243,95}}, {{215, 85}, {228,96}}, {{249, 90}, {264,93}}}, 18, 10}},
    {BULLET_P, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{249, 90}, {264,93}}}, 15, 4}},
    {BULLET_P_CHARGED_20, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{232, 103}, {247,114}}, {{249, 105}, {264, 112}}}, 15, 11}},
    {BULLET_P_CHARGED_40, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{200, 121}, {231,130}}, {{233, 120}, {264, 131}}}, 21, 11}},
    {BULLET_P_CHARGED_60, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{168,137}, {215, 148}}, {{217,136}, {264, 149}}}, 47, 13}},
    {BULLET_P_CHARGED_80, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{136, 154}, {199,167}}, {{201, 154}, {264, 167}}}, 63, 13}},
    {BULLET_P_CHARGED_100, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{104, 171}, {183,184}}, {{185, 170}, {264, 285}}}, 79, 13}},
    {CONCENTRATION_BULLET_CHARGED, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{2, 51}, {32,82}}, {{34, 54}, {62, 79}}, {{68, 54}, {96,81}}, {{103, 55}, {127, 79}}, {{135, 56}, {157, 77}}, {{163, 58}, {193, 75}}, {{200, 59}, {222, 73}}, {{233, 60}, {252, 71}}}, 31, 31}},

    {BONUS_LEVEL_1, {"src/Renderer/assets/sprites/r-typesheet2.gif", {{{279,35}, {298,50}}, {{255,35}, {274,50}}, {{231,35}, {250,50}}, {{207,35}, {226,50}}, {{183,35}, {202,50}}, {{159,35}, {178,50}}}, 19, 15}},
    {BONUS_ITEM, {"src/Renderer/assets/sprites/bonus.png", {{{24,182}, {62,217}}}, 38, 35}},

    {RED_MONSTER_LEFT, {"src/Renderer/assets/sprites/r-typesheet5.gif", {{{236, 10}, {256, 23}}, {{203, 13}, {223,19}}, {{170,12}, {190,25}}, {{137,6}, {157, 29}}, {{104,10}, {124,23}}, {{71,16}, {91,22}}, {{38,12}, {58,25}}, {{5,6}, {25,29}}}, 20, 23}},
    {RED_MONSTER_RIGHT, {"src/Renderer/assets/sprites/r-typesheet5.gif", {{{276,10}, {296,23}}, {{309,13}, {329,19}}, {{342,12}, {362,25}}, {{375,6}, {395, 29}}, {{408,10}, {428,23}}, {{441,16}, {461,22}}, {{474,12}, {494,25}}, {{507,6}, {527,29}}}, 20, 23}},
    {MONSTER_DEATH, {"src/Renderer/assets/sprites/r-typesheet1.gif", {{{247, 304}, {264, 318}}, {{217, 300}, {243, 319}}, {{182, 296}, {213, 322}}, {{146, 296}, {127, 366}}, {{109, 296}, {140, 325}}, {{72, 296}, {103, 325}}}, 31, 29}},
    {MONSTER_DEATH_2, {"src/Renderer/assets/sprites/r-typesheet44.gif", {{{130, 2}, {159, 29}}, {{162, 1}, {192, 32}}, {{194, 1}, {225, 32}}, {{228, 2}, {258, 31}}, {{261, 2}, {291, 31}}, {{294, 1}, {324, 31}}}, 31, 31}},
    {BROWN_MONSTER_LEFT, {"src/Renderer/assets/sprites/r-typesheet7.gif", {{{69,1}, {96,32}}, {{34,4}, {65,32}}, {{1,4}, {32,32}}}, 31, 31}},
    {BROWN_MONSTER_RIGHT, {"src/Renderer/assets/sprites/r-typesheet7.gif", {{{103,1}, {130,32}}, {{134,4}, {165,32}}, {{167,4}, {198,32}}}, 31, 31}},
    {GRAY_MONSTER_LEFT, {"src/Renderer/assets/sprites/r-typesheet11.gif", {{{67,1}, {98,31}}, {{34,2}, {65,31}}, {{1,1}, {32,31}}}, 32, 30}},
    {GRAY_MONSTER_RIGHT, {"src/Renderer/assets/sprites/r-typesheet11.gif", {{{101,1}, {132,31}}, {{134,2}, {165,31}}, {{167,1}, {198,31}}}, 32, 30}},

    {BULLET_M, {"src/Renderer/assets/sprites/r-typesheet43.gif", {{{188,6}, {194,11}}, {{170,6}, {176,11}}, {{153,6}, {159,11}}, {{136,6}, {142,11}}}, 7, 6}},
    {WALL_1, {"src/Renderer/assets/sprites/walls.png", {{{164, 480}, {227,504}}}, 63, 24}},
    {WALL_2, {"src/Renderer/assets/sprites/walls.png", {{{174,0}, {267,46}}}, 93, 46}},
};
