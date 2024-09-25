/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include <iostream>

#include "rtypeClient/RtypeUdpClient.hpp"
#include "rtypeClient/rtypeRfc.hpp"

int main() {
    rtypeNetwork::rtypeUdpClient client("127.0.0.1", 4444);

    network::message<rtypeNetwork::rtypeMessageType> msg;
    msg.header.id = rtypeNetwork::rtypeMessageType::OK;

    msg << "adaaaaam";

    client.sendMessage(msg);

    while (true) sleep(5);
    return 0;
}
