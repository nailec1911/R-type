/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Events
*/

#pragma once

/**
 * @brief Event Key enumeration, any other key should be ignored
 */
#include <sys/types.h>
enum class EventKey : u_int8_t
{
    KeyA,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,
    KeyEscape,
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyEnter,
    KeySpace,
    MouseRight,
    MouseMiddle,
    MouseLeft,
};

/**
 * @brief Event State enumeration, either released or pressed.
 */
enum class EventState : int8_t
{
    KeyReleased,  ///< State for Key release
    KeyPressed,   ///< State for Key press
};

/**
 * @brief Event Container. This contains an eventkey (key) and a state
 */
struct Event
{
    /// Key of the Event
    EventKey key;

    /// State of the Event
    EventState state;
};
