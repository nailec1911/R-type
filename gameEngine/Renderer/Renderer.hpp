/*
** EPITECH PROJECT, 2024
** rts
** File description:
** IRenderer
*/
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <stddef.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <cstdint>
#include <ctime>
#include <vector>
#include "Events.hpp"
#include "IRenderer.hpp"

#pragma once

static const std::map<rndr::Color, sf::Color> conv_color = {
    {rndr::Color::Black, sf::Color::Black},
    {rndr::Color::Maroon, sf::Color::Black},
    {rndr::Color::Green, sf::Color::Green},
    {rndr::Color::Olive, sf::Color::Green},
    {rndr::Color::Navy, sf::Color::Blue},
    {rndr::Color::Purple, sf::Color::Blue},
    {rndr::Color::Teal, sf::Color::Blue},
    {rndr::Color::Silver, sf::Color::White},
    {rndr::Color::Grey, sf::Color::White},
    {rndr::Color::Red, sf::Color::Red},
    {rndr::Color::Lime, sf::Color::Green},
    {rndr::Color::Yellow, sf::Color::Yellow},
    {rndr::Color::Blue, sf::Color::Blue},
    {rndr::Color::Fuchsia, sf::Color::Magenta},
    {rndr::Color::Aqua, sf::Color::Cyan},
    {rndr::Color::White, sf::Color::White},
};

static const std::map<int, EventKey> convert_event{
    {sf::Keyboard::A, EventKey::KeyA},
    {sf::Keyboard::B, EventKey::KeyB},
    {sf::Keyboard::C, EventKey::KeyC},
    {sf::Keyboard::D, EventKey::KeyD},
    {sf::Keyboard::E, EventKey::KeyE},
    {sf::Keyboard::F, EventKey::KeyF},
    {sf::Keyboard::G, EventKey::KeyG},
    {sf::Keyboard::H, EventKey::KeyH},
    {sf::Keyboard::I, EventKey::KeyI},
    {sf::Keyboard::J, EventKey::KeyJ},
    {sf::Keyboard::K, EventKey::KeyK},
    {sf::Keyboard::L, EventKey::KeyL},
    {sf::Keyboard::M, EventKey::KeyM},
    {sf::Keyboard::N, EventKey::KeyN},
    {sf::Keyboard::O, EventKey::KeyO},
    {sf::Keyboard::P, EventKey::KeyP},
    {sf::Keyboard::Q, EventKey::KeyQ},
    {sf::Keyboard::S, EventKey::KeyS},
    {sf::Keyboard::T, EventKey::KeyT},
    {sf::Keyboard::U, EventKey::KeyU},
    {sf::Keyboard::V, EventKey::KeyV},
    {sf::Keyboard::W, EventKey::KeyW},
    {sf::Keyboard::X, EventKey::KeyX},
    {sf::Keyboard::Y, EventKey::KeyY},
    {sf::Keyboard::Z, EventKey::KeyZ},
    {sf::Keyboard::Escape, EventKey::KeyEscape},
    {sf::Keyboard::Up, EventKey::KeyUp},
    {sf::Keyboard::Down, EventKey::KeyDown},
    {sf::Keyboard::Left, EventKey::KeyLeft},
    {sf::Keyboard::Right, EventKey::KeyRight},
    {sf::Keyboard::Enter, EventKey::KeyEnter},
    {sf::Keyboard::Space, EventKey::KeySpace},
    {sf::Mouse::Right, EventKey::MouseRight},
    {sf::Mouse::Middle, EventKey::MouseMiddle},
    {sf::Mouse::Left, EventKey::MouseLeft},
};

class Sprite {
    public:
        Sprite() = default;
        Sprite(std::string filepath, Vector2<float> pos, std::vector<Vector2<Vector2<float>>> frames);
        ~Sprite() = default;

        void setSpritePosition(Vector2<float> newPosition);
        sf::Sprite &getSprite();
        void setDraw(bool draw) {m_display = draw;}
        Vector2<float> getSpritePosition();
        bool isToDraw() {return m_display;}

    protected:
    private:
        std::vector<sf::Sprite> m_sprites;
        bool m_display;
        sf::Texture m_texture;
        size_t m_nbAnim;
        size_t m_indexFrame;
};

class Renderer final : public rndr::IRenderer {
    public:
        Renderer() = delete;
        Renderer(Vector2<int> size = {1920, 1080}, std::string title = "", int frameRate = 60);
        ~Renderer() override {};

        void setBackgrounds(std::string filepath, float speed) override;
        void setPosition(std::uint32_t idSprite, Vector2<float> newPosition) override;
        uint32_t createSprite(uint32_t idEntity, std::string filepath, std::vector<Vector2<Vector2<float>>>frame_pos, Vector2<float>pos) override;
        void removeSprite(uint32_t idSprite) override;
        void moveSprite(uint32_t idSprite, Vector2<float>pos) override;
        void clear(rndr::Color color = rndr::Color::Black) override;
        void refresh() override;
        void hideSprite(uint32_t idSprite) override;
        void drawSprite(uint32_t idSprite) override;
        void drawText(std::string text, Vector2<float>pos, rndr::Color color = rndr::Color::White) override;
        Vector2<float> getPostion(uint32_t idSprite) override {
            return m_spriteMap[idSprite]->getSpritePosition();
        };
        std::vector<Event> getEvents() override;

    protected:
    private:
        void loopBackGround();
        std::unordered_map<uint32_t, std::unique_ptr<Sprite>> m_spriteMap;
        std::string m_background;
        sf::RenderWindow m_windowSFML;
        sf::Texture m_bgTexture;
        std::pair<sf::Sprite, sf::Sprite> m_bgSprites;
        sf::Vector2u m_windowSize;
        sf::Vector2u m_textureSize;
        sf::Clock m_clock;
        float m_deltaTime;
        float m_bgSpeed;
        int m_frameRate;
};
