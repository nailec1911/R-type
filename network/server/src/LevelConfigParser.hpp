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

std::vector<std::string> getAllConfigFile(char const *filepath);

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
        class ErrorLevelParser : public std::exception {
            public:
                ErrorLevelParser(const std::string &msg) : m_msg(msg) {}
                virtual const char *what() const noexcept override
                {
                    return m_msg.c_str();
                }
            private:
                std::string m_msg;
        };

        LevelConfigParser();
        ~LevelConfigParser() = default;
        std::vector<std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>> &getLevel()
        {
            return m_level;
        }
        std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>> &getLevelbyId(int id)
        {
            return m_level.at(id);
        }
        void validateEntity(const YAML::Node& entityNode) 
        {
            static const std::vector<std::string> requiredFields = {
                "type", "spawn_x", "spawn_y"
            };

            for (const auto& field : requiredFields) {
                if (!entityNode[field]) {
                    throw ErrorLevelParser("Error : Missing field '" + field + "' in configuration file.");
                }
            }
        }
   protected:
   private:
    std::vector<std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>> m_level;
};
