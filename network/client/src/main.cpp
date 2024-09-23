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
    network::message<rtypeNetwork::rtypeMessageType> msg1;
    network::message<rtypeNetwork::rtypeMessageType> msg2;
    network::message<rtypeNetwork::rtypeMessageType> msg3;
    msg.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg1.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg2.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg3.header.id = rtypeNetwork::rtypeMessageType::OK;

    int x = 50;
    int y = 0;
    msg << x;
    std::cout << msg << std::endl;
    msg >> y;
    std::cout << "y: " << y << std::endl;

    std::vector<uint8_t> toto;

    msg1 << "cringe1";
    msg2 << "cringe2";
    msg3 << "cringe3";

    msg << toto;

    // msg << msg1;

    client.sendMessage(msg);
    // client.sendMessage(msg1);
    // client.sendMessage(msg2);
    // client.sendMessage(msg3);

    while (true) sleep(5);
    return 0;
}
