/*
** EPITECH PROJECT, 2024
** rts
** File description:
** StructComponent
*/

#pragma once
#include <cstdint>
#include <string>

struct Position
{
    float x;
    float y;
    float initX;
    float initY;
};

struct BoundingBox
{
    float width;
    float height;
};

struct Transform
{
    int16_t velX;
    int16_t velY;
};

class Chrono
{
   public:
    Chrono() = default;
    Chrono(uint32_t tick) : m_tick(tick), m_last_update(tick), m_was_updated(true){};
    Chrono(Chrono &&) = delete;
    Chrono(const Chrono &) = default;
    Chrono &operator=(Chrono &&) = delete;
    Chrono &operator=(const Chrono &) = default;
    ~Chrono() = default;

    [[nodiscard]] uint32_t getLastUpdate() const
    {
        return m_last_update;
    }
    [[nodiscard]] uint32_t getCreateTick() const
    {
        return m_tick;
    }

    void update(uint32_t tick)
    {
        m_last_update = tick;
        m_was_updated = true;
    }

    [[nodiscard]] bool wasUpdated() const {return m_was_updated;}

    void setNotUpdated() {m_was_updated = false;}

   private:
    uint32_t m_tick{};
    uint32_t m_last_update{};
    bool m_was_updated {};
};

struct Player
{
    uint32_t id;
};

struct BulletPlayer
{};

struct BulletMonster
{};

struct Monster
{};

struct ShooterMonster
{};

struct FlyingMonster
{};

struct Wall
{};

struct HUDComp
{
    std::string body;
};
