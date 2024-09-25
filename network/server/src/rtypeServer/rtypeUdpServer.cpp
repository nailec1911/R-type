/*
** EPITECH PROJECT, 2024
** src
** File description:
** rtypeUdpServer
*/

#include "rtypeUdpServer.hpp"

#include <sys/stat.h>

#include <asio/io_context.hpp>
#include <asio/read.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <system_error>

#include "rtypeRfc.hpp"

rtypeNetwork::rtypeUdpServer::rtypeUdpServer(std::uint32_t port)
    : m_socket(m_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      m_readBuffer(),
      m_id(0)
{
    if (m_socket.is_open()) {
        std::cout << "[+]Server started\n";
        m_work = std::make_unique<asio::io_context::work>(m_ctx);
        m_thread = std::thread([&]() { readHeader(), m_ctx.run(); });
    }
}

rtypeNetwork::rtypeUdpServer::~rtypeUdpServer()
{
    m_ctx.stop();
    if (m_thread.joinable())
        m_thread.join();
    if (m_thread.joinable())
        m_thread.join();
    std::cout << "[+]Server closed." << std::endl;
}

void rtypeNetwork::rtypeUdpServer::readBody()
{
    m_socket.async_receive_from(
        asio::buffer(m_tempRead.body.data(), m_tempRead.body.size()),
        m_remoteEndpoint,
        [this](std::error_code ec, [[maybe_unused]] std::size_t bytesRead) {
            if (!ec) {
            } else {
                std::cerr << "[-]Error while reading body." << std::endl;
            }
        });
}

void rtypeNetwork::rtypeUdpServer::readHeader()
{
    m_socket.async_receive_from(
        asio::buffer(
            &m_tempRead.header,
            sizeof(network::messageHeader<rtypeNetwork::rtypeMessageType>)),
        m_remoteEndpoint,
        [this](std::error_code ec, [[maybe_unused]] std::size_t bytesRead) {
            if (!ec) {
                std::string endpointData =
                    m_remoteEndpoint.address().to_string() + ":" +
                    std::to_string(m_remoteEndpoint.port());
                switch (m_tempRead.header.id) {
                    case rtypeNetwork::rtypeMessageType::LOGIN:
                        handleClientLogin(endpointData);
                        break;
                    case rtypeNetwork::rtypeMessageType::MOVE:
                        handleMoveClient(endpointData);
                        break;
                    case rtypeNetwork::rtypeMessageType::SHOOT:
                        handleShootClient(endpointData);
                        break;
                    default:
                        break;
                }
                readHeader();
            } else {
                std::cerr << "[-]Error while reading body." << std::endl;
            }
        });
}

void rtypeNetwork::rtypeUdpServer::handleClientLogin(
    const std::string &endpointData)
{
    if (m_clients.size() >= 5) {
        std::cerr << "There is already 5 players." << std::endl;
        return readBody();
    }
    for (auto &elem : m_clients) {
        if (elem.second.clientAddressPort == endpointData) {
            std::cerr << "Client already connected..." << std::endl;
            return readBody();
        }
    }
    if (m_tempRead.header.size > 0) {
        m_tempRead.body.resize(m_tempRead.header.size);
        readBody();
    }
    m_clients[m_id] = clientSession{
        .clientAddressPort = endpointData,
        .endpoint = m_remoteEndpoint,
        .last_heartbeat = std::chrono::steady_clock::now()};
    m_id++;
}

void rtypeNetwork::rtypeUdpServer::handleMoveClient(
    const std::string &endpointData)
{
    uint16_t clientId = 500;

    for (auto &elem : m_clients) {
        if (elem.second.clientAddressPort == endpointData) {
            clientId = elem.first;
            break;
        }
    }
    if (m_tempRead.header.size > 0) {
        m_tempRead.body.resize(m_tempRead.header.size);
        if (clientId == 500)
            return readBody();
        readBody();
    }
    std::string input(reinterpret_cast<char *>(m_tempRead.body.data()));
    std::cout << "GO PUSH " << input << std::endl;
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

void rtypeNetwork::rtypeUdpServer::handleShootClient(
    const std::string &endpointData)
{
    uint16_t clientId = 500;

    for (auto &elem : m_clients) {
        if (elem.second.clientAddressPort == endpointData) {
            clientId = elem.first;
            break;
        }
    }
    if (m_tempRead.header.size > 0) {
        m_tempRead.body.resize(m_tempRead.header.size);
        if (clientId == 500)
            return readBody();
        readBody();
    }
    m_clientsEvents.push(
        {.id = clientId,
         .event =
             Event{.key = EventKey::KeyB, .state = EventState::KeyPressed}});
}

void rtypeNetwork::rtypeUdpServer::sendMessagesToClient(
    const network::message<rtypeNetwork::rtypeMessageType> &msg,
    uint16_t clientId)
{
}

std::queue<clientEvent> rtypeNetwork::rtypeUdpServer::getClientsEvents(
    void) const
{
    return this->m_clientsEvents;
}
