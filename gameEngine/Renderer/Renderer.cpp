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
#include <memory>
#include <vector>

#include "Events.hpp"
#include "IRenderer.hpp"
#include <iostream>
#include "../../network/client/src/RtypeClient.hpp"

Sprite::Sprite(rndr::elementInfo spriteInfo, rndr::Vector2<float> pos)
    : m_display(true), m_nbAnim(spriteInfo.frames.size())
{
    m_texture.loadFromFile(spriteInfo.filepath);

    for (size_t i = 0; i < m_nbAnim; i++) {
        float width = spriteInfo.frames.at(i).y.x - spriteInfo.frames.at(i).x.x;
        float height = spriteInfo.frames.at(i).y.y - spriteInfo.frames.at(i).x.y;
        sf::IntRect rectSourceSprite(
            spriteInfo.frames.at(i).x.x, spriteInfo.frames.at(i).x.y, width, height);
        sf::Sprite sprite(m_texture, rectSourceSprite);
        sprite.setScale(spriteInfo.scale, spriteInfo.scale);
        sprite.setPosition(pos.x, pos.y);
        sprite.setOrigin(
            static_cast<float>(width) / 2, static_cast<float>(height) / 2);
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
    m_indexFrame += 1;
    if (m_indexFrame == m_nbAnim)
        m_indexFrame = 0;
    return m_sprites.at(m_indexFrame);
}

Renderer::Renderer(rndr::Vector2<int> size, std::string title, int framRate)
    : m_windowSFML(sf::VideoMode(size.x, size.y), title), m_deltaTime(1.0F / static_cast<float>(framRate)), m_frameRate(framRate)
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
    m_windowSFML.display();
}

void Renderer::setPosition(std::uint32_t idSprite, rndr::Vector2<float> newPosition)
{
    m_spriteMap[idSprite]->setSpritePosition(newPosition);
}

uint32_t Renderer::createSprite(
    uint32_t idEntity, rndr::elementInfo spriteInfo, rndr::Vector2<float> pos)
{
    auto sprite = std::make_unique<Sprite>(spriteInfo, pos);
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

void Renderer::drawText(std::string  /*text*/, rndr::Vector2<float>  /*pos*/, rndr::Color  /*color*/)
{
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
