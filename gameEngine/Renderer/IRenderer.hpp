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

using color_t = u_int8_t;
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
    std::vector<Vector2<Vector2<float>>> frames;
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
    virtual uint32_t createSprite(uint32_t idEntity,
        elementInfo spriteInfo, Vector2<float> pos) = 0;
    virtual void removeSprite(uint32_t idSprite) = 0;
    virtual void hideSprite(uint32_t idSprite) = 0;
    virtual void drawSprite(uint32_t idSprite) = 0;
    virtual void moveSprite(uint32_t idSprite, Vector2<float> pos) = 0;
    virtual void clear(Color color = Color::Black) = 0;
    virtual void refresh() = 0;
    virtual void drawText(
        std::string text, Vector2<float> pos, Color color = Color::White) = 0;
    virtual Vector2<float> getPostion(uint32_t idSprite) = 0;
    virtual std::vector<Event> getEvents() = 0;

   protected:
   private:
};
}  // namespace rndr

extern "C" std::unique_ptr<rndr::IRenderer> renderer_entrypoint(void);
