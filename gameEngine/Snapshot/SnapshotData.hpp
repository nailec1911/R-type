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
#include "../Renderer/Elements.hpp"

class SnapshotData
{
   public:
    SnapshotData() = default;
    SnapshotData(const SnapshotData &) = default;
    SnapshotData(SnapshotData &&) = delete;
    SnapshotData &operator=(const SnapshotData &) = default;
    SnapshotData &operator=(SnapshotData &&) = default;
    SnapshotData(elementTypes type, int x, int y, int vx, int vy, int info)
        : m_type(type), m_x(x), m_y(y), m_vx(vx), m_vy(vy), m_info(info){};
    SnapshotData(const std::vector<uint8_t> &bytes, size_t &indx)
        : m_type(static_cast<elementTypes>(extractInt(bytes, indx + 0))),
          m_x(extractInt(bytes, indx + 4)),
          m_y(extractInt(bytes, indx + 8)),
          m_vx(extractInt(bytes, indx + 12)),
          m_vy(extractInt(bytes, indx + 16)),
          m_info(extractInt(bytes, indx + 20))
    {
          indx += 24;
    }
    ~SnapshotData() = default;

    operator std::vector<uint8_t>() const
    {
        std::vector<uint8_t> res;
        res.push_back(m_type);
        res.push_back(m_x);
        res.push_back(m_y);
        res.push_back(m_vx);
        res.push_back(m_vy);
        res.push_back(m_info);
        return res;
    };

    bool operator==(const SnapshotData &b) const
    {
        return m_x == b.m_x && m_y == b.m_y && m_vx == b.m_vx &&
               m_vy == b.m_vy && m_info == b.m_info;
    }

   private:
    static int extractInt(const std::vector<uint8_t> &bytes, size_t offset)
    {
        int value = 0;
        for (size_t i = 0; i < sizeof(int); ++i) {
            value |= static_cast<int>(bytes[offset + i]) << (i * 8);
        }
        return value;
    }

    elementTypes m_type;
    int m_x;
    int m_y;
    int m_vx;
    int m_vy;
    int m_info;
};
