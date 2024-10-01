/*
** EPITECH PROJECT, 2024
** rts
** File description:
** inputTomessage
*/

#include <vector>

#include "../../../../gameEngine/Renderer/Events.hpp"
#include "../RtypeClient.hpp"

std::vector<asun::message<CustomMessageType>> inputToMessage(
    const std::vector<Event>& events)
{
    std::vector<asun::message<CustomMessageType>> eventMessages;
    for (auto elem : events) {
        asun::message<CustomMessageType> newMsg;
        switch (elem.key)
        {
        case EventKey::KeyUp:
            newMsg.header.id = CustomMessageType::MOVE;
            newMsg << "1";
            break;
        case EventKey::KeyDown:
            newMsg.header.id = CustomMessageType::MOVE;
            newMsg << "3";
            break;
        case EventKey::KeyLeft:
            newMsg.header.id = CustomMessageType::MOVE;
            newMsg << "4";
            break;
        case EventKey::KeyRight:
            newMsg.header.id = CustomMessageType::MOVE;
            newMsg << "2";
            break;
        case EventKey::KeyB:
            newMsg.header.id = CustomMessageType::SHOOT;
            break;
        default:
            newMsg.header.id = CustomMessageType::NONE;
            break;
        }
        eventMessages.push_back(newMsg);
    }
    return eventMessages;
}
