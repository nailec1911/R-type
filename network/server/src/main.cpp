/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#ifdef __linux__
    #include <unistd.h>
#endif

#include <cstdint>
#include <exception>

#include "LevelConfigParser.hpp"
#include "RtypeServer.hpp"

int main(int argc, char **argv)
{
    try {
        checkParametersServer(argc, argv);
        rtypeNetwork::RtypeServer server(
            static_cast<uint16_t>(std::stoi(argv[1])), 5);  // NOLINT
        server.start();
        while (true)
            server.handleMessages();
    } catch (const ErrorParams &p) {
        std::cerr << p.what() << std::endl;
        return 84;
    } catch (const LevelConfigParser::ErrorLevelParser &l) {
        std::cerr << l.what() << std::endl;
        return 84;
    } catch (const rtypeNetwork::RtypeServer::ErrorRtypeServer &s) {
        std::cerr << s.what() << std::endl;
        return 84;
    } catch (const HelpExceptionServer &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error, exiting program..." << std::endl;
        std::cerr << error.what() << std::endl;
        return 84;
    }
    return 0;
}
