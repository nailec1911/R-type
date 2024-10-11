/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Errors
*/
#include <string>

#pragma once

constexpr int ERROR_IP = 101;
constexpr int ERROR_PORT = 102;
int checkParametersClient(int argc, char** argv);
int checkParametersServer(int argc, char** argv);

class Error : public std::exception
{
   public:
    Error(const std::string& msg, int code) : message(msg), errorCode(code) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

    int getCode() const
    {
        return errorCode;
    }

   private:
    std::string message;
    int errorCode;
};

class ErrorParams : public Error
{
   public:
    ErrorParams(int code) : Error(getMessageFromCode(code), code) {}

    static std::string getMessageFromCode(int code)
    {
        switch (code) {
            case ERROR_IP:
                return "Invalid IP address specified.";
            case ERROR_PORT:
                return "Invalid port specified.";
            default:
                return "Unknown parameter error.";
        }
    }
};

class HelpExceptionClient : public std::exception
{
   public:
    const char* what() const noexcept override
    {
        return (
            "Usage: ./Client [IP ADDRESS] [PORT]\n"
            "    IP ADDRESS: The IP address of the server to connect to.\n"
            "    PORT: The port on which the server will listen for incoming "
            "connections.\n"
            "    Place your Entity configuration file in the configEntities folder\n"
            "    Place your Level configuration file in the configLevels folder");
    }
};

class HelpExceptionServer : public std::exception
{
   public:
    const char* what() const noexcept override
    {
        return (
            "Usage: ./Server [PORT]\n"
            "    PORT: The port on which the server will listen for incoming "
            "connections.\n"
            "Place your Entity configuration file in the configEntities folder\n"
            "Place your Level configuration file in the configLevels folder");
    }
};
