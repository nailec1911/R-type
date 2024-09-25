/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "rtypeClient/RtypeUdpClient.hpp"
#include "rtypeClient/rtypeRfc.hpp"

int main()
{
    rtypeNetwork::rtypeUdpClient client("127.0.0.1", 4444);

    network::message<rtypeNetwork::rtypeMessageType> msg;
    network::message<rtypeNetwork::rtypeMessageType> msg2;
    msg.header.id = rtypeNetwork::rtypeMessageType::LOGIN;
    msg2.header.id = rtypeNetwork::rtypeMessageType::MOVE;

    msg << "adamudesu";
    msg2 << "1";

    client.sendMessage(msg);

    while (true) {
        sleep(5);
        client.sendMessage(msg2);
    }
    return 0;
}
