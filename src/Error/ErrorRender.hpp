/*
** EPITECH PROJECT, 2024
** rts
** File description:
** ErrorRender
*/

#ifndef ERRORRENDER_HPP_
#define ERRORRENDER_HPP_
#include <string>

class ErrorRender {
    public:
        ErrorRender(std::string errorMsg) : _msg(errorMsg)
        {
        }

        const char *what() const noexcept
        {
            return _msg.c_str();
        }
        std::string _msg;
    protected:
    private:
};

#endif /* !ERRORRENDER_HPP_ */
