/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Snapshot
*/

#pragma once

#include <format>
#ifdef __linux__
    #include <unistd.h>
#endif

#include <zstd.h>
#include <zstd_errors.h>

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

enum class CustomMessageType
{
    SHOOT,
    MOVE,
    OK,
    KO,
    LOGIN,
    LOGOUT,
    SNAPSHOT,
    SNAP_OK,
    DEAD,
    WIN,
    YOUR_PLAYER,
    NONE,
};

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
    T, std::void_t<decltype(T(
           std::declval<const std::vector<uint8_t> &>(),
           std::declval<size_t &>()))>> : std::true_type
{};

template <typename Telement, int nb_others>
// typename Telement, typename Tother, int nb_others, uint32_t sizeTother>
class Snapshot
{
    static_assert(
        has_equal_operator<Telement>::value,
        "Telement must have operator== overloaded");
    static_assert(
        has_vector_conversion<Telement>::value,
        "Telement must have cast operator std::vector<uint8_t> overloaded");
    static_assert(
        has_vector_constructor<Telement>::value,
        "Telement must have constructo Telement(const std::vector<uint8_t>&, "
        "size_t &)");
    // static_assert(
    // has_equal_operator<Tother>::value,
    // "Tother must have operator== overloaded");
    // static_assert(
    // has_vector_conversion<Tother>::value,
    // "Tother must have cast operator std::vector<uint8_t> overloaded");

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
        uint32_t id, std::array<int, nb_others> otherInfo,
        std::unordered_map<uint32_t, Telement> eltsMap,
        std::bitset<nb_others> otherUpadte = std::bitset<nb_others>().flip())
        : m_otherUpadte(otherUpadte),
          m_otherInfo(otherInfo),
          m_eltsMap(eltsMap),
          m_timestamp(time(0)),
          m_id(id){};
    ~Snapshot() = default;
    Snapshot(const std::vector<uint8_t> &bytes)
    {
        auto msg = uncompressData(bytes);

        if (msg.at(0)  != m_startSnap || msg.back() != m_endSnap)
            throw std::format_error("Snapchots is corrupted");

        size_t indx = 1;
        m_id = extractUint32(msg, indx);
        for (int i = 0; i < nb_others; i++) {
            if (msg.at(indx) == 0) {
                indx += 1;
                continue;
            }
            indx += 1;
            m_otherInfo[i] = extractUint32(msg, indx);
        }
        uint32_t nbEntity = extractUint32(msg, indx);
        for (size_t i = 0; i < nbEntity; i++) {
            uint32_t id = extractUint32(msg, indx);
            try {
                m_eltsMap[id] = Telement(msg, indx);
            } catch (const std::out_of_range &e) {
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
    const std::array<int, nb_others> &getOthers() const
    {
        return m_otherInfo;
    }
    const std::unordered_map<uint32_t, Telement> &getElements() const
    {
        return m_eltsMap;
    }

    std::vector<uint8_t> toMessage()
    {
        std::vector<uint8_t> res{intToVector(m_id)};

        for (int i = 0; i < nb_others; i += 1) {
            if (m_otherUpadte[i]) {
                auto toAdd = intToVector(m_otherInfo.at(i));
                res.push_back(1);
                res.insert(res.end(), toAdd.begin(), toAdd.end());
            } else {
                res.push_back(0);
            }
        }

        auto nbElts = intToVector(m_eltsMap.size());
        res.insert(res.end(), nbElts.begin(), nbElts.end());
        for (auto &elt : m_eltsMap) {
            auto eltId = intToVector(elt.first);
            res.insert(res.end(), eltId.begin(), eltId.end());
            auto toAdd = static_cast<std::vector<uint8_t>>(elt.second);
            res.insert(res.end(), toAdd.begin(), toAdd.end());
        }
        res.push_back(m_endSnap);
        res.insert(res.begin(), m_startSnap);
        return compressData(res);
    }

   protected:
   private:
    std::bitset<nb_others> m_otherUpadte;
    std::array<int, nb_others> m_otherInfo;
    std::unordered_map<uint32_t, Telement> m_eltsMap;
    std::time_t m_timestamp{};
    uint32_t m_id{};
    bool m_acknowledge{};
    static constexpr uint8_t m_startSnap = 231;
    static constexpr uint8_t m_endSnap = 123;


    std::vector<uint8_t> compressData(
        const std::vector<uint8_t> &serializedData)
    {
        size_t maxCompressedSize = ZSTD_compressBound(serializedData.size());
        std::vector<uint8_t> compressedData(maxCompressedSize);
        size_t compressedSize = ZSTD_compress(
            compressedData.data(), maxCompressedSize, serializedData.data(),
            serializedData.size(), 1);
        if (ZSTD_isError(compressedSize) != 0)
            throw std::runtime_error("Compression failed");
        compressedData.resize(compressedSize);
        return compressedData;
    }

    std::vector<uint8_t> uncompressData(
        const std::vector<uint8_t> &compressedData)
    {
        size_t decompressedSize = ZSTD_getFrameContentSize(
            compressedData.data(), compressedData.size());

        if (decompressedSize == ZSTD_CONTENTSIZE_ERROR)
            throw std::runtime_error(
                "Invalid compressed data: cannot determine original size");
        if (decompressedSize == ZSTD_CONTENTSIZE_UNKNOWN)
            throw std::runtime_error("Compressed data size is unknown");

        std::vector<uint8_t> decompressedData(decompressedSize);
        size_t result = ZSTD_decompress(
            decompressedData.data(), decompressedSize, compressedData.data(),
            compressedData.size());

        if (ZSTD_isError(result) != 0)
            throw std::runtime_error("Decompression failed");
        return decompressedData;
    }

    static std::vector<uint8_t> intToVector(int value)
    {
        std::vector<uint8_t> vec;

        for (size_t i = 0; i < sizeof(int); ++i) {
            vec.push_back(static_cast<uint8_t>(value >> (8 * i)));
        }
        return vec;
    }

    template <typename T>
    static uint32_t extractValue(
        const std::vector<uint8_t> &bytes, size_t &offset, size_t typeSize)
    {
        T value = 0;
        for (size_t i = 0; i < typeSize; ++i) {
            value |= bytes[offset + i] << (i * 8);
        }
        offset += sizeof(typeSize);
        return value;
    }

    static uint32_t extractUint32(
        const std::vector<uint8_t> &bytes, size_t &offset)
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
