/*
** EPITECH PROJECT, 2024
** rts
** File description:
** inputTomessage
*/
#include "../RtypeClient.hpp"
#include <vector>
#include "../../../../gameEngine/Renderer/Events.hpp"

std::vector<asun::message<rtypeNetwork::CustomMessageType>> inputToMessage(const std::vector<Event>& events)
{
    std::vector<asun::message<rtypeNetwork::CustomMessageType>> eventMessages;
    for (auto elem: events) {
        asun::message<rtypeNetwork::CustomMessageType> newMsg;
        switch (elem.key)
        {
        case EventKey::KeyUp:
            newMsg.header.id = rtypeNetwork::CustomMessageType::MOVE;
            newMsg << "1";
            break;
        case EventKey::KeyDown:
            newMsg.header.id = rtypeNetwork::CustomMessageType::MOVE;
            newMsg << "3";
            break;
        case EventKey::KeyLeft:
            newMsg.header.id = rtypeNetwork::CustomMessageType::MOVE;
            newMsg << "4";
            break;
        case EventKey::KeyRight:
            newMsg.header.id = rtypeNetwork::CustomMessageType::MOVE;
            newMsg << "2";
            break;
        case EventKey::KeyB:
            newMsg.header.id = rtypeNetwork::CustomMessageType::SHOOT;
            break;
        default:
            break;
        }
        eventMessages.push_back(newMsg);
    }
    return eventMessages;
}