/*
** EPITECH PROJECT, 2024
** rts
** File description:
** IRenderer
*/

#pragma once

#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "Events.hpp"

namespace rndr {

/**
 * @brief Vector for 2 Dimensional reference
 * @tparam T Type of the vector
 */
template <typename T>
struct Vector2
{
    T x;  ///< x value
    T y;  ///< y value
};

struct Frame
{
    rndr::Vector2<float> topLeft;
    rndr::Vector2<float> bottomRight;
};

using color_t = uint8_t;

enum class Color : color_t
{
    Black,
    Maroon,
    Green,
    Olive,
    Navy,
    Purple,
    Teal,
    Silver,
    Grey,
    Red,
    Lime,
    Yellow,
    Blue,
    Fuchsia,
    Aqua,
    White
};

struct elementInfo
{
    std::string filepath;
    std::vector<Frame> frames;
    int sizeX;
    int sizeY;
    float scale;
};

class IRenderer
{
   public:
    class Exception : public std::exception
    {};

    IRenderer() = default;
    IRenderer(const IRenderer &) = default;
    IRenderer(IRenderer &&) = delete;
    IRenderer &operator=(const IRenderer &) = default;
    IRenderer &operator=(IRenderer &&) = delete;
    virtual ~IRenderer() = default;

    virtual void setBackgrounds(std::string filepath, float speed) = 0;
    virtual void setPosition(
        std::uint32_t idSprite, Vector2<float> newPosition) = 0;
    virtual uint32_t createSprite(
        uint32_t idEntity, const std::string &srpiteRef,
        Vector2<float> pos) = 0;
    virtual void removeSprite(uint32_t idSprite) = 0;
    virtual void hideSprite(uint32_t idSprite) = 0;
    virtual void drawSprite(uint32_t idSprite) = 0;
    virtual void drawText(const std::string &text) = 0;
    virtual void display() = 0;
    virtual void moveSprite(uint32_t idSprite, Vector2<float> pos) = 0;
    virtual void clear(Color color = Color::Black) = 0;
    virtual void refresh() = 0;
    virtual Vector2<float> getPostion(uint32_t idSprite) = 0;
    virtual std::vector<Event> getEvents() = 0;
    virtual bool isWindowOpen() = 0;

   protected:
   private:
};
}  // namespace rndr

#ifdef __linux__
    extern "C" std::unique_ptr<rndr::IRenderer> renderer_entrypoint(void);
#else
    std::unique_ptr<rndr::IRenderer> renderer_entrypoint(void);
#endif
