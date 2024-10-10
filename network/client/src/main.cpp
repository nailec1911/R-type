/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "../../../gameEngine/Renderer/ConfigParser.hpp"
#include "../../../gameEngine/Renderer/Renderer.hpp"
#include "../../Message.hpp"
#include "RtypeClient.hpp"

std::vector<asun::message<CustomMessageType>> inputToMessage(
    const std::vector<Event>& events);

int main(int argc, char** argv)
{
    if (!checkParameters(argc, argv))
        return 84;
    Renderer renderer({1920, 1080}, "Rtype");
    ConfigParser config("../configEntity.yml");
    renderer.setEltInfo(config.getEltInfo());
    asun::message<CustomMessageType> msg{};
    rtypeNetwork::RtypeClient client(argv[1], std::stoi(argv[2]));
    msg.header.id = CustomMessageType::LOGIN;
    std::vector<asun::message<CustomMessageType>> eventMessage;
    std::vector<Event> events;

    renderer.setBackgrounds(
        "../gameEngine/Renderer/assets/background_5.png", 50);
    client.start();
    client.sendMessage(msg);
    while (renderer.isWindowOpen()) {
        client.handleMessages(renderer);
        renderer.clear();
        events = renderer.getEvents();
        eventMessage = inputToMessage(events);
        renderer.refresh();
        if (client.isPlayerDead())
            renderer.drawText(
                "You're dead.", rndr::Vector2<float>(420, 400), 200);
        if (client.hasPlayerWon())
            renderer.drawText("You won!", rndr::Vector2<float>(500, 400), 200);
        renderer.display();
        for (const auto& elem : eventMessage) {
            client.sendMessage(elem);
        }
    }
    return 0;
}
