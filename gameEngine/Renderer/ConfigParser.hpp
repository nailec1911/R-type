/*
** EPITECH PROJECT, 2024
** rts
** File description:
** ConfigParser
*/

#ifndef CONFIGPARSER_HPP_
#define CONFIGPARSER_HPP_

#include "IRenderer.hpp"
#include <unordered_map>
#include <yaml-cpp/yaml.h>



inline std::vector<rndr::Frame> parseFrames(const YAML::Node& framesNode) {
    std::vector<rndr::Frame> frames;

    for (std::size_t i = 0; i < framesNode.size(); ++i) {
        rndr::Frame frame{};
        frame.topLeft = {framesNode[i]["start"][0].as<float>(), framesNode[i]["start"][1].as<float>()};
        frame.bottomRight = {framesNode[i]["end"][0].as<float>(), framesNode[i]["end"][1].as<float>()};
        frames.push_back(frame);
    }

    return frames;
}

class ConfigParser {
    public:
        ConfigParser(const std::string &filename);
        ~ConfigParser() = default;
        std::vector<std::string> getEntityType()
        {
            return m_entitiesType;
        }
        std::unordered_map<std::string, rndr::elementInfo> getEltInfo()
        {
            return m_eltInfo;
        }

    protected:
    private:
        std::vector<std::string>m_entitiesType;
        std::unordered_map<std::string, rndr::elementInfo> m_eltInfo;
};

#endif /* !CONFIGPARSER_HPP_ */
