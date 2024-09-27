/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Snapshot
*/

#pragma once

#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gameServer {

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
    T,
    std::void_t<decltype(static_cast<std::vector<uint8_t>>(std::declval<T>()))>>
    : std::true_type
{};

template <typename, typename = std::void_t<>>
struct has_vector_constructor : std::false_type
{};

template <typename T>
struct has_vector_constructor<
    T,
    std::void_t<decltype(T(std::declval<const std::vector<uint8_t>&>(), std::declval<size_t&>()))>>
    : std::true_type {};

template <typename Telement, typename Tother, int nb_others, uint32_t sizeTother>
class Snapshot
{
    static_assert(
        has_equal_operator<Telement>::value,
        "Telement must have operator== overloaded");
    static_assert(
        has_vector_conversion<Telement>::value,
        "Telement must have cast operator std::vector<uint8_t> overloaded");
    static_assert(
        has_equal_operator<Tother>::value,
        "Tother must have operator== overloaded");
    static_assert(
        has_vector_conversion<Tother>::value,
        "Tother must have cast operator std::vector<uint8_t> overloaded");

   public:
    Snapshot()
        : m_otherUpadte(std::bitset<nb_others>().flip()),
          m_otherInfo({0}),
          m_eltsMap({0}),
          m_timestamp(time(0)){};
    Snapshot(const Snapshot &) = default;
    Snapshot(Snapshot &&) = default;
    Snapshot &operator=(const Snapshot &) = default;
    Snapshot &operator=(Snapshot &&) = default;
    Snapshot(
        uint32_t id, std::array<Tother, nb_others> otherInfo,
        std::unordered_map<uint32_t, Telement> m_eltsMap,
        std::bitset<nb_others> otherUpadte = std::bitset<nb_others>().flip())
        : m_otherUpadte(otherUpadte),
          m_otherInfo(otherInfo),
          m_eltsMap(m_eltsMap),
          m_timestamp(time(0)),
          m_id(id){};
    ~Snapshot() = default;
    Snapshot(const std::vector<uint8_t> &bytes)
    {
        int indx = 0;

        for (int i = 0; i < nb_others; i++) {
            if (bytes.at(indx) == 0) {
                indx += 1;
                continue;
            }
            indx += 1;
            Tother value{bytes.begin() + indx, bytes.begin() + indx + sizeTother};
            indx += sizeTother;
        }
        uint32_t nbEntity = extractUint32(bytes, indx);

        for (size_t i = 0; i < nbEntity; i++) {
            uint32_t id = extractUint32(bytes, indx);
            try {
                m_eltsMap[id] = Telement(bytes, indx);
            } catch (const std::out_of_range& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "Parsing failed" << std::endl;
            }
        }
    }

    [[nodiscard]] time_t getCreationTime() const
    {
        return m_timestamp;
    }
    [[nodiscard]] bool isAcknowledged() const
    {
        return m_acknowledge;
    }
    void setAcknowledge()
    {
        m_acknowledge = true;
    }
    [[nodiscard]] uint32_t getId() const
    {
        return m_id;
    }
    const std::array<Tother, nb_others> &getOthers() const
    {
        return m_otherInfo;
    }
    const std::unordered_map<uint32_t, Telement> &getElements() const
    {
        return m_eltsMap;
    }

    std::vector<uint8_t> toMessage()
    {
        std::vector<uint8_t> res{static_cast<std::vector<uint8_t>>(m_id)};

        for (int i = 0; i < nb_others; i += 1) {
            if (m_otherUpadte[i]) {
                auto toAdd = static_cast<std::vector<uint8_t>>(m_otherInfo.at(i));
                res.push_back(1);
                res.insert(res.end(), toAdd.begin(), toAdd.end());
            } else {
                res.push_back(0);
            }
        }

        res.push_back(static_cast<uint32_t>(m_eltsMap.size()));
        for (auto &elt : m_eltsMap) {
            res.push_back(elt.first);
            auto toAdd = static_cast<std::vector<uint8_t>>(elt.second);
            res.insert(res.end(), toAdd.begin(), toAdd.end());
        }
        return res;
    }

   protected:
   private:
    std::bitset<nb_others> m_otherUpadte;
    std::array<Tother, nb_others> m_otherInfo;
    std::unordered_map<uint32_t, Telement> m_eltsMap;
    std::time_t m_timestamp{};
    uint32_t m_id{};
    bool m_acknowledge{};

    static uint32_t extractUint32(const std::vector<uint8_t> &bytes, int &offset)
    {
        uint32_t value = 0;
        for (size_t i = 0; i < sizeof(uint32_t); ++i) {
            value |= static_cast<uint32_t>(bytes[offset + i]) << (i * 8);
        }
        offset += sizeof(uint32_t);
        return value;
    }
};
};  // namespace gameServer
