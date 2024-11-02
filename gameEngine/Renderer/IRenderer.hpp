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
#include <unordered_map>
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

/**
 * @brief struct for a Frame of a sprite animation
 *
 */
struct Frame
{
    rndr::Vector2<float> topLeft;  ///< top left corner
    rndr::Vector2<float> bottomRight; ///< bottom left corner
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

/**
 * @brief struct to store the renderer's information of a sprite
 *
 */
struct elementInfo
{
    std::string filepath; ///> path of the image file
    std::vector<Frame> frames; ///> vector of the different frames for animation, if no animation, just put one frame
    int sizeX; ///> size x of the sprite
    int sizeY; ///> size y of the sprite
    float scale; ///> scale of the sprite
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

    /**
     * @brief Set the Backgrounds of the window, the background should loop slowly and permanently
     *
     * @param filepath path of the background image
     * @param speed speed (in pixel per seconds) at which the background should move
     */
    virtual void setBackgrounds(std::string filepath, float speed) = 0;

    /**
     * @brief Set the Position of a sprite
     *
     * @param idSprite id of the sprite
     * @param newPosition new position {x, y}
     */
    virtual void setPosition(
        std::uint32_t idSprite, Vector2<float> newPosition) = 0;

    /**
     * @brief Create a Sprite object. This function use the element info from the config to set up the sprite
     *
     * @param idEntity Id of the sprite to create, the user must ensure he doesn't use the same id twice
     * @param srpiteRef ref od the sprite in the config file
     * @param pos initial position of the sprite
     * @return uint32_t id of the created sprite
     */
    virtual uint32_t createSprite(
        uint32_t idEntity, const std::string &srpiteRef,
        Vector2<float> pos) = 0;

    /**
     * @brief delete a sprite
     *
     * @param idSprite id of the sprite to delete
     */
    virtual void removeSprite(uint32_t idSprite) = 0;

    /**
     * @brief move a sprite at a new position
     *
     * @param idSprite id of the sprite to move
     * @param pos new sprite position {x, y}
     */
    virtual void moveSprite(uint32_t idSprite, Vector2<float> pos) = 0;

    /**
     * @brief clear the window
     *
     * @param color color to put after clear, default as black as some libs do not implement it
     */
    virtual void clear(Color color = Color::Black) = 0;

    /**
     * @brief refresh the renderer state
     *
     */
    virtual void refresh() = 0;

    /**
     * @brief hide a sprite, if a sprite is hiden, he should not be displayed
     *
     * @param idSprite id of the sprite to hide
     */
    virtual void hideSprite(uint32_t idSprite) = 0;

    /**
     * @brief draw a sprite for the next frame
     *
     * @param idSprite id of the sprite to draw
     */
    virtual void drawSprite(uint32_t idSprite) = 0;


    /**
     * @brief draw the text at his preset position,
     *
     * @param text
     */
    virtual void drawText(const std::string &text) = 0;

    /**
     * @brief display the renderer state
     *
     */
    virtual void display() = 0;

    /**
     * @brief Get the Postion of a sprite
     *
     * @param idSprite id of the sprite
     * @return Vector2<float> pos x and y
     */
    virtual Vector2<float> getPostion(uint32_t idSprite) = 0;

    /**
     * @brief hcek if the renderer window is open
     *
     * @return true > window open
     * @return false > window closed
     */
    virtual bool isWindowOpen() = 0;

    /**
     * @brief Get the vector of Events object, it should contains all the events that happend since the last frame
     *
     * @return std::vector<Event> all the events since last frame
     */
    virtual std::vector<Event> getEvents() = 0;

    /**
     * @brief Set the Elt Info, the element info should be use when a sprite is created, the ref allows to search throught the infos
     *
     * @param newEltInfo map of : ref of sprite, sprites info
     */
    virtual void setEltInfo(const std::unordered_map<std::string, rndr::elementInfo> &newEltInfo) = 0;
   protected:
   private:
};
}  // namespace rndr

#ifdef __linux__
    extern "C" std::unique_ptr<rndr::IRenderer> renderer_entrypoint(void);
#else
    std::unique_ptr<rndr::IRenderer> renderer_entrypoint(void);
#endif
