/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "rtypeClient/RtypeUdpClient.hpp"
#include "rtypeClient/rtypeRfc.hpp"
#include <iostream>

int main()
{
    rtypeNetwork::rtypeUdpClient client("127.0.0.1", 4444);

    network::message<rtypeNetwork::rtypeMessageType> msg;
    network::message<rtypeNetwork::rtypeMessageType> msg1;
    network::message<rtypeNetwork::rtypeMessageType> msg2;
    network::message<rtypeNetwork::rtypeMessageType> msg3;
    msg.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg1.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg2.header.id = rtypeNetwork::rtypeMessageType::OK;
    msg3.header.id = rtypeNetwork::rtypeMessageType::OK;

    msg << "cringe";
    msg1 << "cringe1";
    msg2 << "cringe2";
    msg3 << "cringe3";

    msg << msg1;

    std::cout << msg  << std::endl;

    client.sendMessage(msg);
    //client.sendMessage(msg1);
    //client.sendMessage(msg2);
    //client.sendMessage(msg3);

    while (true)
        sleep(5);
    return 0;
}
