/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Sprite
*/
#include "Renderer.hpp"

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