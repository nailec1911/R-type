/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "RtypeClient.hpp"
#include "../../../gameEngine/Renderer/Renderer.hpp"

#include "../../Message.hpp"

int main()
{
    Renderer renderer({1920, 1080}, "Rtype");
    asun::message<rtypeNetwork::CustomMessageType> msg{};
    rtypeNetwork::RtypeClient client("127.0.0.1", 4444);
    msg.header.id = rtypeNetwork::CustomMessageType::LOGIN;
    msg << "marius";
    std::vector<asun::message<rtypeNetwork::CustomMessageType>> eventMessage;
    std::vector<Event> events;
    renderer.setBackgrounds("../gameEngine/Renderer/assets/background_5.png", 50);

    client.start();
    client.sendMessage(msg);
    while (renderer.isWindowOpen())
    {
        renderer.clear();
        events = renderer.getEvents();
        eventMessage = inputToMessage(events);
        renderer.refresh();
        for (const auto& elem : eventMessage) {
            client.sendMessage(elem);
            std::cout << elem << std::endl;
        }
    }
    return 0;
}
