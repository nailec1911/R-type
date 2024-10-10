/*
** EPITECH PROJECT, 2024
** rts
** File description:
** LevelConfigParser
*/

#pragma once

#include <yaml-cpp/yaml.h>

#include <string>
#include <iostream>
#include <unordered_map>

#include "../../../gameEngine/ECS/using.hpp"

static const std::unordered_map<std::string, EntityName> stringToEnumMap = {
    {"NONE", NONE},
    {"PLAYER", PLAYER},
    {"P_BULLET", P_BULLET},
    {"WALL", WALL},
    {"MONSTER", MONSTER},
    {"HUD", HUD},
    {"P_BULLET_CHARGED", P_BULLET_CHARGED},
    {"M_BULLET", M_BULLET},
    {"TRANSFORM", TRANSFORM},
    {"POSITION", POSITION},
    {"BOUNDING_BOX", BOUNDING_BOX},
    {"SHOOTER_MONSTER", SHOOTER_MONSTER},
    {"FLYING_MONSTER", FLYING_MONSTER}};

struct entitySpawn
{
    EntityName type;
    std::pair<float, float> vecPos;
};

class LevelConfigParser
{
   public:
    LevelConfigParser(const std::string &filename)
    {
        YAML::Node config = YAML::LoadFile(filename);
        for (const auto &elem : config["timeline"]) {
            float time = elem["time"].as<float>();
            if (elem["entities"]) {
                for (const auto &enemyNode : elem["entities"]) {
                    entitySpawn enemy;
                    enemy.type =
                        stringToEnumMap.at(enemyNode["type"].as<std::string>());
                    enemy.vecPos.first = enemyNode["spawn_x"].as<float>();
                    enemy.vecPos.second = enemyNode["spawn_y"].as<float>();
                    level[time].push_back(enemy);
                }
            }
        }
    }
    ~LevelConfigParser() = default;
    std::unordered_map<float, std::vector<entitySpawn>> &getLevel()
    {
        return level;
    }
    void displayLevelConfig()
    {
        for (const auto &entry : level) {
            float time = entry.first;
            const std::vector<entitySpawn> &entities = entry.second;
            std::cout << "Time: " << time << std::endl;
            for (const auto &entity : entities) {
                std::cout << "Entity Type: " << entity.type
                          << " Spawn X: " << entity.vecPos.first
                          << " Spawn Y: " << entity.vecPos.second << std::endl;
            }
        }
    }

   protected:
   private:
    std::unordered_map<float, std::vector<entitySpawn>> level;
};
