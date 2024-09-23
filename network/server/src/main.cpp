/*
** EPITECH PROJECT, 2024
** server
** File description:
** main
*/

#include "rtypeServer/rtypeUdpServer.hpp"
#include <unistd.h>
#include <iostream>

int main()
{
    rtypeNetwork::rtypeUdpServer server(4444);
    while (true)
    {
        sleep(60);
        std::cout << "Async proof" << std::endl;
    }
    return 0;
}
