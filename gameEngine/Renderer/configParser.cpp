/*
** EPITECH PROJECT, 2024
** rts
** File description:
** configParser
*/
#include <iostream>
#include <vector>
#include "ConfigParser.hpp"
#include "Events.hpp"
#include "IRenderer.hpp"

ConfigParser::ConfigParser()
{
    std::vector<std::string> allConfigFiles = getAllConfigFile("../configEntities");

    for (const auto &item : allConfigFiles) {
        checkFile(item);
        YAML::Node config = YAML::LoadFile(item);
        std::vector<std::string>entitiesType;

        if (config["background"] && m_background.empty())
            m_background = config["background"].as<std::string>();
        if (!config["Entities"])
            throw ErrorParser("None Entities to parse");
        for (YAML::const_iterator it = config["Entities"].begin(); it != config["Entities"].end(); ++it) {
            std::string entityName = it->first.as<std::string>();
            entitiesType.push_back(entityName);
        }
        for (auto &elem : entitiesType) {
            rndr::elementInfo newElem;
            validateEntity(config["Entities"][elem]);
            newElem.filepath = config["Entities"][elem]["sprite"].as<std::string>();
            checkfileSprite(newElem.filepath);
            newElem.scale = config["Entities"][elem]["scale"].as<float>();
            newElem.sizeX = config["Entities"][elem]["width"].as<int>();
            newElem.sizeY = config["Entities"][elem]["height"].as<int>();
            YAML::Node framesNode = config["Entities"][elem]["frames"];
            newElem.frames = parseFrames(framesNode);
            m_eltInfo[elem] = newElem;
        }
        m_entitiesType.insert(m_entitiesType.end(), entitiesType.begin(), entitiesType.end());
    }
    if (m_background.empty())
        throw ErrorParser("No background specified");
}

inline void ConfigParser::validateFrameNode(const YAML::Node& frameNode) 
{
    if (!frameNode["start"] || !frameNode["end"]) {
        throw ErrorParser("Error: Missing 'start' or 'end' field in frame.");
    }
    
    if (frameNode["start"].size() != 2 || frameNode["end"].size() != 2) {
        throw ErrorParser("Error: 'start' and 'end' must contain exactly 2 elements.");
    }
}

inline std::vector<rndr::Frame> ConfigParser::parseFrames(const YAML::Node& framesNode) 
{
    std::vector<rndr::Frame> frames;

    for (const auto& frameNode : framesNode) {
        validateFrameNode(frameNode);
        rndr::Frame frame{};
        frame.topLeft = {
            frameNode["start"][0].as<float>(),
            frameNode["start"][1].as<float>()
        };
        frame.bottomRight = {
            frameNode["end"][0].as<float>(),
            frameNode["end"][1].as<float>()
        };
        frames.push_back(frame);
    }
    return frames;
}

void ConfigParser::validateEntity(const YAML::Node& entityNode) 
{
    static const std::vector<std::string> requiredFields = {
        "sprite", "scale", "width", "height", "frames"
    };

    for (const auto& field : requiredFields) {
        if (!entityNode[field]) {
            throw ErrorParser("Error : Missing field '" + field + "' in configuration file.");
        }
    }
}

void ConfigParser::checkfileSprite(const std::string &filepath)
{
    if (!std::filesystem::exists(filepath)) {
        throw ErrorParser("The file which contain the sprite hasn't been found : " + filepath);
    }
}