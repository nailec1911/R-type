/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "RtypeClient.hpp"

#include "../../Message.hpp"

int main()
{
    asun::message<rtypeNetwork::CustomMessageType> msg{};
    rtypeNetwork::RtypeClient client("127.0.0.1", 4444);
    msg.header.id = rtypeNetwork::CustomMessageType::OK;
    msg << "mariu";

    client.start();
    client.sendMessage(msg);
    while (true)
    {
        sleep(1);
        std::cout << "async proof" << std::endl;
    }
    return 0;
}
