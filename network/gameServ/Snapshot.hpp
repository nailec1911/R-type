/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Snapshot
*/

#pragma once

#include <sys/types.h>

#include <array>
#include <bitset>
#include <cstdint>
#include <ctime>
#include <map>
#include <utility>
#include <vector>

namespace gameServer {

// class SnapElement {
// public:
//   SnapElement(int x, int y, int vx, int vy, int info)
//   : m_x(x), m_y(y), m_vx(vx), m_vy(vy), m_info(info){};
//   SnapElement(const std::vector<u_int8_t> &bytes) {
// m_x = extractInt(bytes, 0);
// m_y = extractInt(bytes, 4);
// m_vx = extractInt(bytes, 8);
// m_vy = extractInt(bytes, 12);
// m_info = extractInt(bytes, 16);
//   }
//   ~SnapElement();
//
//   operator std::vector<u_int8_t>() const {
// std::vector<u_int8_t> res;
// res.push_back(m_x);
// res.push_back(m_y);
// res.push_back(m_vx);
// res.push_back(m_vy);
// res.push_back(m_info);
// return res;
//   };
//
//   bool operator==(const SnapElement &b) const {
// return m_x == b.m_x && m_y == b.m_y && m_vx == b.m_vx && m_vy == b.m_vy &&
//    m_info == b.m_info;
//   }
//
// private:
//   static int extractInt(const std::vector<uint8_t> &bytes, size_t offset) {
// int value = 0;
// for (size_t i = 0; i < sizeof(int); ++i) {
//   value |= static_cast<int>(bytes[offset + i]) << (i * 8);
// }
// return value;
//   }
//
//   int m_x;
//   int m_y;
//   int m_vx;
//   int m_vy;
//   int m_info;
// };

template <typename, typename = std::void_t<>>
struct has_equal_operator : std::false_type
{};

template <typename T>
struct has_equal_operator<
    T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
    : std::true_type
{};

template <typename, typename = std::void_t<>>
struct has_vector_conversion : std::false_type
{};

template <typename T>
struct has_vector_conversion<
    T, std::void_t<decltype(std::declval<T>().operator std::vector<uint8_t>())>>
    : std::true_type
{};

template <typename Telement, typename Tother, int nb_others>
class Snapshot
{
    static_assert(
        has_equal_operator<Telement>::value,
        "Telement must have operator== overloaded");
    static_assert(
        has_vector_conversion<Telement>::value,
        "Telement must have cast operator std::vector<u_int8_t> overloaded");
    static_assert(
        has_equal_operator<Tother>::value,
        "Tother must have operator== overloaded");
    static_assert(
        has_vector_conversion<Tother>::value,
        "Tother must have cast operator std::vector<u_int8_t> overloaded");

   public:
    Snapshot();
    Snapshot(const Snapshot &) = delete;
    Snapshot(Snapshot &&) = delete;
    Snapshot &operator=(const Snapshot &) = delete;
    Snapshot &operator=(Snapshot &&) = delete;
    Snapshot(
        int id, std::array<Tother, nb_others> otherInfo,
        std::map<u_int32_t, Telement> m_eltsMap,
        std::bitset<nb_others> otherUpadte = std::bitset<nb_others>().flip())
        : m_otherUpadte(otherUpadte),
          m_otherInfo(otherInfo),
          m_eltsMap(m_eltsMap),
          m_timestamp(time(0)),
          m_id(id),
          m_acknowledge(false){};

    ~Snapshot();

    time_t getCreationTime() const
    {
        return m_timestamp;
    }
    bool isAcknowledged() const
    {
        return m_acknowledge;
    }
    void setAcknowledge()
    {
        m_acknowledge = true;
    }
    int getId() const
    {
        return m_id;
    }
    std::array<Tother, nb_others> &getOthers() const
    {
        return m_otherInfo;
    }
    std::map<u_int32_t, Telement> &getElements() const
    {
        return m_eltsMap;
    }

    std::vector<u_int8_t> toMessage()
    {
        std::vector<u_int8_t> res{m_id};

        for (int i = 0; i < nb_others; i += 1) {
            if (m_otherUpadte[i]) {
                std::vector<u_int8_t> toAdd =
                    static_cast<std::vector<u_int8_t>>(m_otherInfo);
                res.insert(res.end(), toAdd.begin(), toAdd.end());
            } else {
                res.push_back(0);
            }
        }

        res.push_back(static_cast<int>(m_eltsMap.size()));
        for (auto &elt : m_eltsMap) {
            res.push_back(elt.first);
            std::vector<u_int8_t> toAdd =
                static_cast<std::vector<u_int8_t>>(*elt.second);
            res.insert(res.end(), toAdd.begin(), toAdd.end());
        }
        return res;
    }

   protected:
   private:
    std::bitset<nb_others> m_otherUpadte;
    std::array<Tother, nb_others> m_otherInfo;
    std::map<u_int32_t, Telement> m_eltsMap;
    std::time_t m_timestamp;
    int m_id;
    bool m_acknowledge;
};
};  // namespace gameServer
