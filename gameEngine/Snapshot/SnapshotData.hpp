/*
** EPITECH PROJECT, 2024
** rts
** File description:
** SnapshotData
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "../ECS/using.hpp"
#include "../Renderer/IRenderer.hpp"

class SnapshotData
{
   public:
    SnapshotData() = default;
    SnapshotData(const SnapshotData &) = default;
    SnapshotData(SnapshotData &&) = delete;
    SnapshotData &operator=(const SnapshotData &) = default;
    SnapshotData &operator=(SnapshotData &&) = default;

    SnapshotData(
        EntityName type, int16_t x, int16_t y, float speed_x,
        float speed_y, uint32_t tick, int info)
        : m_type(type),
          m_x(x),
          m_y(y),
          m_speedX(speed_x),
          m_speedY(speed_y),
          m_tick(tick),
          m_destroy(info){};
    SnapshotData(const std::vector<uint8_t> &bytes, size_t &indx)
        : m_type(static_cast<EntityName>(extractVal<int>(bytes, indx))),
          m_x(extractVal<int16_t>(bytes, indx)),
          m_y(extractVal<int16_t>(bytes, indx)),
          m_speedX(extractVal(bytes, indx)),
          m_speedY(extractVal(bytes, indx)),
          m_tick(extractVal<uint32_t>(bytes, indx)),
          m_destroy(extractVal<int>(bytes, indx))
    {
    }

    ~SnapshotData() = default;

    operator std::vector<uint8_t>() const
    {
        std::vector<uint8_t> res;

        appendToVector(res, static_cast<int>(m_type));
        appendToVector(res, m_x);
        appendToVector(res, m_y);
        appendToVector(res, m_speedX);
        appendToVector(res, m_speedY);
        appendToVector(res, m_tick);
        appendToVector(res, m_destroy);

        return res;
    }

    bool operator==(const SnapshotData &b) const
    {
        return m_x == b.m_x && m_y == b.m_y && m_speedX == b.m_speedX &&
               m_speedY == b.m_speedY && m_destroy == b.m_destroy;
    }

    rndr::Vector2<int> getXY()
    {
        return {m_x, m_y};
    }

    EntityName getType()
    {
        return m_type;
    }

    rndr::Vector2<float> getVelocity() const
    {
        return {m_speedX, m_speedY};
    }

    [[nodiscard]] int getDestroy() const
    {
        return m_destroy;
    }

    [[nodiscard]] uint32_t getTick() const
    {
        return m_tick;
    }

   private:
    template <typename T>
    static T extractVal(const std::vector<uint8_t> &bytes, size_t &offset)
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");

        T value = 0;
        for (size_t i = 0; i < sizeof(T); ++i) {
            value |= static_cast<T>(bytes[offset]) << (i * 8);
            offset += 1;
        }
        return value;
    }

    static float extractVal(const std::vector<uint8_t> &bytes, size_t &offset)
    {
        static_assert(sizeof(float) == 4, "Float must be 4 bytes");

        float value = 0.0F;
        std::memcpy(&value, &bytes[offset], sizeof(float));
        offset += sizeof(float);
        return value;
    }

    template <typename T>
    static void appendToVector(std::vector<uint8_t> &res, T value)
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");

        for (size_t i = 0; i < sizeof(T); ++i) {
            res.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
        }
    }

    static void appendToVector(std::vector<uint8_t> &res, float value)
    {
        static_assert(sizeof(float) == 4, "Float must be 4 bytes");

        uint8_t byteArray[sizeof(float)];
        std::memcpy(byteArray, &value, sizeof(float));
        for (size_t i = 0; i < sizeof(float); ++i) {
            res.push_back(byteArray[i]);
        }
    }

    EntityName m_type;
    int16_t m_x;
    int16_t m_y;
    float m_speedX;
    float m_speedY;
    uint32_t m_tick;
    int m_destroy;
};
