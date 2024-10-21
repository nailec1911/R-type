/*
** EPITECH PROJECT, 2024
** rts
** File description:
** LevelConfigParser
*/

#include "LevelConfigParser.hpp"

LevelConfigParser::LevelConfigParser()
{
    std::vector<std::string> allConfigLevelFiles =
        getAllConfigFile("../configLevels");

    for (const auto &elem : allConfigLevelFiles) {
        std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>
            parsedLevel;
        YAML::Node config = YAML::LoadFile(elem);
        if (!config["timeline"])
            throw ErrorLevelParser("No timeline specified");
        for (const auto &elem : config["timeline"]) {
            if (!elem["time"])
                throw ErrorLevelParser("No time for entity spawn specified");
            float time = elem["time"].as<float>();
            for (const auto &enemyNode : elem["entities"]) {
                entitySpawn enemy;
                this->validateEntity(enemyNode);
                enemy.type =
                    stringToEnumMap.at(enemyNode["type"].as<std::string>());
                enemy.vecPos.first = enemyNode["spawn_x"].as<float>();
                enemy.vecPos.second = enemyNode["spawn_y"].as<float>();
                parsedLevel.second[time].push_back(enemy);
            }
        }
        if (!config["win_condition"])
            throw ErrorLevelParser("no win condition specified");
        parsedLevel.first = config["win_condition"].as<float>();
        m_level.push_back(parsedLevel);
    }
}
