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

Renderer::Renderer(rndr::Vector2<int> size, std::string title, int frameRate)
    : m_windowSFML(sf::VideoMode(size.x, size.y), title),
      m_deltaTime(1.0F / static_cast<float>(frameRate)),
      m_frameRate(frameRate)
{
    const std::string fragmentSource = R"(
        uniform sampler2D texture;
        uniform int mode;

        void main() {
            vec4 color = texture2D(texture, gl_TexCoord[0].xy);
            vec4 defaultColor = color;
            if (mode == 0) {
                gl_FragColor = color;
                return;
            }
            float L = (17.8824 * color.r) + (43.5161 * color.g) + (4.11935 * color.b);
            float M = (3.45565 * color.r) + (27.1554 * color.g) + (3.86714 * color.b);
            float S = (0.0299566 * color.r) + (0.184309 * color.g) + (1.46709 * color.b);

            float l, m, s;
            if (mode == 0) { //Default
                gl_FragColor = defaultColor;
                return;
            } else if (mode == 1) {            // Protanopia
                l = 0.0 * L + 2.02344 * M + -2.52581 * S;
                m = 0.0 * L + 1.0 * M + 0.0 * S;
                s = 0.0 * L + 0.0 * M + 1.0 * S;
            } else if (mode == 2) {     // Deuteranopia
                l = 1.0 * L + 0.0 * M + 0.0 * S;
                m = 0.494207 * L + 0.0 * M + 1.24827 * S;
                s = 0.0 * L + 0.0 * M + 1.0 * S;
            } else if (mode == 3) {     // Tritanopia
                l = 1.0 * L + 0.0 * M + 0.0 * S;
                m = 0.0 * L + 1.0 * M + 0.0 * S;
                s = -0.395913 * L + 0.801109 * M + 0.0 * S;
            }

            vec4 error;
            error.r = (0.0809444479 * l) + (-0.130504409 * m) + (0.116721066 * s);
            error.g = (-0.0102485335 * l) + (0.0540193266 * m) + (-0.113614708 * s);
            error.b = (-0.000365296938 * l) + (-0.00412161469 * m) + (0.693511405 * s);
            error.a = 1.0;

            vec4 diff = color - error;

            vec4 correction;
            correction.r = 0.0;
            correction.g = (diff.r * 0.7) + (diff.g * 1.0);
            correction.b = (diff.r * 0.7) + (diff.b * 1.0);
            correction = color + correction;
            correction.a = color.a;

            gl_FragColor = correction;
        }
    )";
     if (!m_colorBlindShader.loadFromMemory(fragmentSource, sf::Shader::Fragment))
        throw ErrorRenderer("Error in shader load !");
    m_colorBlindShader.setUniform("texture", sf::Shader::CurrentTexture);
    m_colorBlindShader.setUniform("mode", 0);

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
            m_windowSFML.draw(item.second->getSprite(),  &m_colorBlindShader);
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
    m_windowSFML.draw(m_spriteMap[idSprite]->getSprite(),  &m_colorBlindShader);
}

void Renderer::drawText(const std::string &text)
{
    m_windowSFML.draw(m_textMap[text]->getText(),  &m_colorBlindShader);
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
    m_windowSFML.draw(m_bgSprites.first, &m_colorBlindShader);
    m_windowSFML.draw(m_bgSprites.second, &m_colorBlindShader);
}

std::vector<Event> Renderer::getEvents()
{
    std::vector<Event> events = {};
    sf::Event event{};

    while (m_windowSFML.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                events.push_back({convert_event.at(sf::Keyboard::Q), EventState::KeyPressed});
                break;
            default:
                break;
        }
    }
    for (std::pair<int, EventKey> item : convert_event) {
        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(item.first);
        if (sf::Keyboard::isKeyPressed(key)) {
            if (event.key.code == sf::Keyboard::Num1)
                m_colorBlindShader.setUniform("mode", 1); // Protanopia
            if (event.key.code == sf::Keyboard::Num2)
                m_colorBlindShader.setUniform("mode", 2); // Deuteranopia
            if (event.key.code == sf::Keyboard::Num3)
                m_colorBlindShader.setUniform("mode", 3); // Tritanopia
            if (event.key.code == sf::Keyboard::Num0)
                m_colorBlindShader.setUniform("mode", 0);
            events.push_back({convert_event.at(key), EventState::KeyPressed});
        }
    }
    return events;
}
