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
#include <filesystem>

std::vector<std::string> getAllConfigFile(char const *filepath);

class ConfigParser {
    public:
        class ErrorParser : std::exception {
            public:
                ErrorParser(const std::string &msg) : m_msg(msg) {}
                const char *what() const noexcept override
                {
                    return m_msg.c_str();
                }
            private:
                std::string m_msg;
        };
        ConfigParser();
        ~ConfigParser() = default;
        std::vector<std::string> getEntityType()
        {
            return m_entitiesType;
        }
        std::unordered_map<std::string, rndr::elementInfo> getEltInfo()
        {
            return m_eltInfo;
        }
        std::string getBackground()
        {
            return m_background;
        }

    protected:
    private:
        static inline std::vector<rndr::Frame> parseFrames(const YAML::Node& framesNode);
        static void validateEntity(const YAML::Node& entityNode);
        static inline void validateFrameNode(const YAML::Node& frameNode);
        static void checkfileSprite(const std::string &filepath);
        void checkFile(const std::string &filepath)
        {
            if (!std::filesystem::exists(filepath)) {
                throw ConfigParser::ErrorParser("file not found : " + filepath);
            }
            if (!std::filesystem::is_regular_file(filepath)) {
                throw ConfigParser::ErrorParser("This is not a regular file : " + filepath);
            }
            if (!isYmlFile(filepath))
            throw ConfigParser::ErrorParser("This is not a YAML file : " + filepath);
        }
        std::string getFileExtension(const std::string &filepath)
        {
            size_t dotPosition = filepath.find_last_of('.');

            if (dotPosition == std::string::npos)
                return "";
            return filepath.substr(dotPosition + 1);
        }
        bool isYmlFile(const std::string& filename)
        {
            std::string extension = getFileExtension(filename);
            return extension == "yml" || extension == "yaml";
        }
        std::vector<std::string>m_entitiesType;
        std::unordered_map<std::string, rndr::elementInfo> m_eltInfo;
        std::string m_background;
};
#endif /* !CONFIGPARSER_HPP_ */
