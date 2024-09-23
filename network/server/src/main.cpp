/*
** EPITECH PROJECT, 2024
** server
** File description:
** main
*/

#include "AsioUdpServer.hpp"
#include <unistd.h>
#include <iostream>

int main()
{
    network::AsioUdpServer udpServer(4444);
    while (true)
    {
        sleep(1);
        std::cout << "Async proof" << std::endl;
    }
    return 0;
}
