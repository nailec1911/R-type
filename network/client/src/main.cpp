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

static
void gameloop(Renderer &renderer,  rtypeNetwork::RtypeClient &client, ConfigParser &config)
{
    std::vector<asun::message<CustomMessageType>> eventMessage;
    std::vector<Event> events;
    asun::message<CustomMessageType> msg{};
    msg.header.id = CustomMessageType::LOGIN;

    renderer.setBackgrounds(
        config.getBackground(), 50);
    client.start();
    client.sendMessage(msg);
    while (renderer.isWindowOpen()) {
        client.handleMessages(renderer);
        renderer.clear();
        events = renderer.getEvents();
        eventMessage = inputToMessage(events);
        renderer.refresh();
        if (client.isPlayerDead()) {
            renderer.getTextMap()["DEAD"] = std::make_unique<Text>(std::string("You're dead"), rndr::Vector2<float>(500, 400), 200);
            renderer.drawText("DEAD");
        }
        if (renderer.getTextMap().find("DEAD") != renderer.getTextMap().end())
            std::cout << renderer.getTextMap().at("DEAD")->getTextPosition().x << std::endl;
        if (client.hasPlayerWon()) {
            renderer.getTextMap()["WIN"] = std::make_unique<Text>("You won", rndr::Vector2<float>(500, 400), 200);
            renderer.drawText("WIN");
        }
        renderer.display();
        for (const auto& elem : eventMessage) {
            client.sendMessage(elem);
        }
    }
}

int main(int argc, char** argv)
{
    try {
        checkParametersClient(argc, argv);
        Renderer renderer({1920, 1080}, "Rtype");
        ConfigParser config;
        renderer.setEltInfo(config.getEltInfo());
        rtypeNetwork::RtypeClient client(argv[1], std::stoi(argv[2]));
        gameloop(renderer, client, config);
    } catch (const ErrorParams &p) {
        std::cout << p.what() << std::endl;
        return 84;
    } catch (const ConfigParser::ErrorParser &c) {
        std::cout << c.what() << std::endl;
        return 84;
    } catch (const Renderer::ErrorRenderer &r) {
        std::cout << r.what() << std::endl;
        return 84;
    } catch (const HelpExceptionClient &e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception &err ) {
        std::cout << "Error : exiting program..." << std::endl;
        return 84;
    }
    return 0;
}
