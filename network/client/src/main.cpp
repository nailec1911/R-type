/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "AsioUdpClient.hpp"

int main()
{
    network::AsioUdpClient client("127.0.0.1", 4444);
    network::message<network::messageType> msg;
    msg.header.id = network::messageType::OK;
    msg.header.size = 0;
    msg << "Mariu\n";
    client.sendMessage(msg);
    return 0;
}
