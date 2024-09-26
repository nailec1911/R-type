/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeServer
*/

#include "RtypeServer.hpp"

#include <asio/ip/udp.hpp>
#include <string>

void rtypeNetwork::RtypeServer::handleMessages()
{
    std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> msg;
    while (getSizeReadQueue() > 0) {
        msg = popReadQueue();
        switch (msg.second.header.id) {
            case CustomMessageType::LOGIN:
                handleClientLogin(msg);
                break;
            case CustomMessageType::MOVE:
                handleClientMove(msg);
                break;
            case CustomMessageType::SHOOT:
                handleClientShoot(msg);
                break;
            default:
                break;
        }
    }
}

void rtypeNetwork::RtypeServer::handleClientLogin(
    std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg)
{
    if (m_clients.size() >= 5) {
        std::cerr << "There is already 5 players." << std::endl;
        return;
    }
    std::string endpointData = msg.first.address().to_string() + ":" +
                               std::to_string(msg.first.port());
    for (auto &elem : m_clients) {
        if (elem.second.clientProtocol == endpointData) {
            std::cerr << "Client already connected..." << std::endl;
            return;
        }
    }
    m_clients[m_id] = clientSession{
        .clientProtocol = endpointData,
        .endpoint = msg.first,
        .lastHeartBeat = std::chrono::steady_clock::now()};
    m_id++;
}

void rtypeNetwork::RtypeServer::handleClientMove(
    std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg)
{
    uint16_t clientId = 500;
    std::string endpointData = msg.first.address().to_string() + ":" +
                               std::to_string(msg.first.port());

    for (auto &elem : m_clients) {
        if (elem.second.clientProtocol == endpointData) {
            clientId = elem.first;
            break;
        }
    }
    if (clientId == 500)
        return;
    std::string input(reinterpret_cast<char *>(msg.second.body.data()));
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
    std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg)
{
    uint16_t clientId = 500;
    std::string endpointData = msg.first.address().to_string() + ":" +
                               std::to_string(msg.first.port());

    for (auto &elem : m_clients) {
        if (elem.second.clientProtocol == endpointData) {
            clientId = elem.first;
            break;
        }
    }
    if (clientId == 500)
        return;
    m_clientsEvents.push(
        {.id = clientId,
         .event =
             Event{.key = EventKey::KeyB, .state = EventState::KeyPressed}});
}