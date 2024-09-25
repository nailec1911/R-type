/*
** EPITECH PROJECT, 2024
** server
** File description:
** main
*/

#include <unistd.h>

#include <iostream>

#include "rtypeServer/rtypeUdpServer.hpp"

int main() {
    rtypeNetwork::rtypeUdpServer server(4444);
    while (true) {
        sleep(1);
        std::cout << "Async proof" << std::endl;
    }
    return 0;
}
