#pragma once

#include <iostream>
#include <sys/socket.h>

using String = std::string;

class Client {
    public:
        Client();
        void    init(int socket);
        void    destroy();
        String  getName();
        int     getSocket();
        void    sendMessage(Client& from, String message);
        bool    isclosed();
        void    readMessage(String message);
    private:
        int socket;
        String name;
};
