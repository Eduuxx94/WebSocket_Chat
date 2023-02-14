#include "Client.hpp"

String Client::getName() {
    return this->name;
}

Client::Client(): socket(0) {

}

int Client::getSocket() {
    return this->socket;
}

void  Client::sendMessage(Client& from,const String message) {
    if (!message.find("--nickname "))
        return ;
    String output = "\r";
    output = from.getName();
    output += ": ";
    output += "\033[0;32m";
    output += message;
    output += "\033[0m";
    send(this->socket, output.c_str(), output.length(), 0);
}

void    Client::init(int socket) {
    this->socket = socket;
    this->name = (String("User") + std::to_string(this->socket));
}

void    Client::destroy() {
    this->socket = 0;
}

bool    Client::isclosed() {
    return this->socket > 0;
}

void    Client::readMessage(String message) {
    if (!message.find("--nickname ")) {
        if (message.length() > 14) {
            this->name = message.substr(11, message.length());
            this->name = name.erase(name.length() - 1);
            std::cout << "Client " << this->socket << " Updated the name for: " << this->name << std::endl;
        }
    }
}
