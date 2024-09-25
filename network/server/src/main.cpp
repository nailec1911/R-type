/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "RtypeServer.hpp"

int main()
{
    rtypeNetwork::RtypeServer server(4444);
    server.start();
    while (true)
    {
        sleep(1);
        std::cout << "async proof" << std::endl;
    }
    return 0;
}
