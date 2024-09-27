/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeServer
*/

#include "RtypeServer.hpp"
#include <asio/ip/udp.hpp>
#include <cstdint>
#include <string>

void rtypeNetwork::RtypeServer::handleMessages()
{
    auto msgQueue = m_gameServ.getMessages();
    while (!msgQueue.empty()) {
        auto msg = msgQueue.front();
        uint32_t clientId = msg.first;
        auto content = msg.second;
        msgQueue.pop();
        std::cout << msg.second << std::endl;
        if (content.header.id == CustomMessageType::LOGIN) {
            handleClientLogin(clientId, content);
            continue;
        }
        if (!isClientConnected(clientId)) {
            std::cerr << "why is this client not connected" << clientId << std::endl;
            continue;
        }
        switch (content.header.id) {
            case CustomMessageType::MOVE:
                handleClientMove(clientId, content);
                break;
            case CustomMessageType::SHOOT:
                handleClientShoot(clientId, content);
                break;
            default:
                break;
        }
    }
}

void rtypeNetwork::RtypeServer::handleClientLogin(
    uint32_t clientId, asun::message<CustomMessageType> & /*msg*/)
{
    if (isClientConnected(clientId)) {
        std::cerr << "already logged in" << std::endl;
        return;
    }
    if (m_gameServ.getNbClient() < m_maxClient) {
        std::cout << "client added" << std::endl;
        m_connectedClients.emplace_back(clientId);
        return;
    }
    std::cerr << "There is already 5 players." << std::endl;
    m_gameServ.removeClient(clientId);
}

void rtypeNetwork::RtypeServer::handleClientMove(
    uint32_t clientId, asun::message<CustomMessageType> &msg)
{
    std::string input(reinterpret_cast<char *>(msg.body.data()));
    if (input == "1")
        m_clientsEvents.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyUp, .state = EventState::KeyPressed}});
    if (input == "2")
        m_clientsEvents.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyRight, .state = EventState::KeyPressed}});
    if (input == "3")
        m_clientsEvents.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyDown, .state = EventState::KeyPressed}});
    if (input == "4")
        m_clientsEvents.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyLeft, .state = EventState::KeyPressed}});
}

void rtypeNetwork::RtypeServer::handleClientShoot(
    uint32_t clientId, asun::message<CustomMessageType> & /*msg*/)
{
    m_clientsEvents.push(
        {.id = clientId,
         .event =
             Event{.key = EventKey::KeyB, .state = EventState::KeyPressed}});
}
