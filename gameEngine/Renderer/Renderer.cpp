/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Renderer
*/

#include "Renderer.hpp"

#include <sys/types.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
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

Sprite::Sprite(
    rndr::elementInfo spriteInfo, rndr::Vector2<float> pos, size_t frameRate)
    : m_display(true),
      m_nbAnim(spriteInfo.frames.size()),
      m_frameRate(frameRate)
{
    m_texture.loadFromFile(spriteInfo.filepath);

    for (size_t i = 0; i < m_nbAnim; i++) {
        float width = spriteInfo.frames.at(i).bottomRight.x -
                      spriteInfo.frames.at(i).topLeft.x;
        float height = spriteInfo.frames.at(i).bottomRight.y -
                       spriteInfo.frames.at(i).topLeft.y;
        sf::IntRect rectSourceSprite(
            static_cast<int>(spriteInfo.frames.at(i).topLeft.x),
            static_cast<int>(spriteInfo.frames.at(i).topLeft.y),
            static_cast<int>(width), static_cast<int>(height));
        sf::Sprite sprite(m_texture, rectSourceSprite);
        sprite.setScale(spriteInfo.scale, spriteInfo.scale);
        sprite.setPosition(pos.x, pos.y);
        // sprite.setOrigin(
        // static_cast<float>(width) / 2, static_cast<float>(height) / 2);
        m_sprites.push_back(sprite);
    }
}

rndr::Vector2<float> Sprite::getSpritePosition()
{
    sf::Vector2f pos = m_sprites.at(0).getPosition();

    return {static_cast<float>(pos.x), static_cast<float>(pos.y)};
}

void Sprite::setSpritePosition(rndr::Vector2<float> newPosition)
{
    for (size_t i = 0; i < m_nbAnim; i++) {
        m_sprites.at(i).setPosition(newPosition.x, newPosition.y);
    }
}

void Sprite::setCustomScale(float newScale)
{
    for (size_t i = 0; i < m_nbAnim; i++) {
        m_sprites.at(i).setScale(newScale, newScale);
    }
}

sf::Sprite &Sprite::getSprite()
{
    if (m_nbAnim == 1)
        return m_sprites.at(0);
    if (m_triggerNextFrame == m_frameRate / 4) {
        m_indexFrame += 1;
        m_triggerNextFrame = 0;
    } else
        m_triggerNextFrame += 1;
    if (m_indexFrame == m_nbAnim)
        m_indexFrame = 0;
    return m_sprites.at(m_indexFrame);
}

Renderer::Renderer(rndr::Vector2<int> size, std::string title, int framRate)
    : m_windowSFML(sf::VideoMode(size.x, size.y), title),
      m_deltaTime(1.0F / static_cast<float>(framRate)),
      m_frameRate(framRate)
{
    m_windowSFML.setFramerateLimit(m_frameRate);
    m_windowSize = m_windowSFML.getSize();
}

void Renderer::setBackgrounds(std::string filepath, float speed)
{
    m_bgTexture.loadFromFile(filepath);
    m_bgSprites.first.setTexture(m_bgTexture);
    m_bgSprites.second.setTexture(m_bgTexture);
    m_bgSprites.first.setPosition(0, 0);
    m_bgSprites.second.setPosition(
        m_bgSprites.first.getGlobalBounds().width, 0);
    m_bgSpeed = speed;
}

void Renderer::refresh()
{
    loopBackGround();
    for (auto &item : m_spriteMap) {
        if (item.second->isToDraw()) {
            m_windowSFML.draw(item.second->getSprite());
        }
    }
}

void Renderer::display()
{
    m_windowSFML.display();
}

void Renderer::setPosition(
    std::uint32_t idSprite, rndr::Vector2<float> newPosition)
{
    m_spriteMap[idSprite]->setSpritePosition(newPosition);
}

uint32_t Renderer::createSprite(
    uint32_t idEntity, const std::string &spriteRef, rndr::Vector2<float> pos)
{
    auto sprite =
        std::make_unique<Sprite>(m_eltInfo[spriteRef], pos, m_frameRate);
    m_spriteMap[idEntity] = std::move(sprite);
    return idEntity;
}

void Renderer::removeSprite(uint32_t idSprite)
{
    m_spriteMap.erase(idSprite);
}

void Renderer::drawSprite(uint32_t idSprite)
{
    m_spriteMap[idSprite]->setDraw(true);
    m_windowSFML.draw(m_spriteMap[idSprite]->getSprite());
}

void Renderer::hideSprite(uint32_t idSprite)
{
    m_spriteMap[idSprite]->setDraw(false);
}

void Renderer::moveSprite(uint32_t idSprite, rndr::Vector2<float> pos)
{
    m_spriteMap[idSprite]->setSpritePosition(pos);
}

void Renderer::clear(rndr::Color color)
{
    m_windowSFML.clear(conv_color.at(color));
}

void Renderer::drawText(
    std::string text, rndr::Vector2<float> pos, int size, rndr::Color /*color*/)
{
    sf::Font font;
    if (!font.loadFromFile(
            "../gameEngine/Renderer/assets/PixelifySans-Regular.ttf")) {
        std::cerr << "Error occured when trying to load font." << std::endl;
        return;
    }
    sf::Text newText;
    newText.setString(text);
    newText.setFont(font);
    newText.setCharacterSize(size);
    newText.setFillColor(sf::Color(255, 255, 255, 128));
    newText.setPosition(pos.x, pos.y);
    m_windowSFML.draw(newText);
}

void Renderer::loopBackGround()
{
    if (m_bgSprites.first.getPosition().x +
            m_bgSprites.first.getGlobalBounds().width <=
        0) {
        m_bgSprites.first.setPosition(
            m_bgSprites.second.getPosition().x +
                m_bgSprites.second.getGlobalBounds().width,
            0);
    }
    if (m_bgSprites.second.getPosition().x +
            m_bgSprites.second.getGlobalBounds().width <=
        0) {
        m_bgSprites.second.setPosition(
            m_bgSprites.first.getPosition().x +
                m_bgSprites.first.getGlobalBounds().width,
            0);
    }
    m_bgSprites.first.move(-m_bgSpeed * m_deltaTime, 0);
    m_bgSprites.second.move(-m_bgSpeed * m_deltaTime, 0);
    m_windowSFML.draw(m_bgSprites.first);
    m_windowSFML.draw(m_bgSprites.second);
}

std::vector<Event> Renderer::getEvents()
{
    std::vector<Event> events = {};
    sf::Event event{};

    while (m_windowSFML.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_windowSFML.close();
                break;
            default:
                break;
        }
    }
    for (std::pair<int, EventKey> item : convert_event) {
        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(item.first);
        if (sf::Keyboard::isKeyPressed(key))
            events.push_back({convert_event.at(key), EventState::KeyPressed});
    }

    return events;
}
