/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "../../../gameEngine/Renderer/Renderer.hpp"
#include "RtypeClient.hpp"

#include "../../Message.hpp"

std::vector<asun::message<CustomMessageType>> inputToMessage(const std::vector<Event>& events);

int main()
{
    Renderer renderer({1920, 1080}, "Rtype");
    asun::message<CustomMessageType> msg{};
    rtypeNetwork::RtypeClient client("127.0.0.1", 4444);
    msg.header.id = CustomMessageType::LOGIN;
    msg << "marius";
    std::vector<asun::message<CustomMessageType>> eventMessage;
    std::vector<Event> events;
    renderer.setBackgrounds("../gameEngine/Renderer/assets/background_5.png", 50);

    client.start();
    client.sendMessage(msg);
    while (renderer.isWindowOpen())
    {
        client.handleMessages(renderer);
        renderer.clear();
        events = renderer.getEvents();
        eventMessage = inputToMessage(events);
        renderer.refresh();
        for (const auto& elem : eventMessage) {
            client.sendMessage(elem);
        }
    }
    return 0;
}
