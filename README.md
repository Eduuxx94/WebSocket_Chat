# WebSocket_Chat
This repository is a C++ socket server using the standard library. This implementation creates a socket on the specified port and listens for incoming connections.

Please READ all below:

This server does not contain any security mechanisms. To make this implementation safer, you should add the necessary security measures such as SSL/TLS, limiting the number of connections, and validating incoming data to prevent buffer overflows, etc.

    Lack of input validation: This code does not validate input from clients, making it vulnerable to buffer overflow attacks.
    Lack of authentication: There is no authentication mechanism to identify clients, so anyone can connect to the server.
    Lack of encryption: All data sent and received over the network is in plain text, making it vulnerable to eavesdropping attacks.
    Use of a fixed-size buffer: The buffer used to receive data from clients is of fixed size, which can lead to buffer overflow vulnerabilities.
    Lack of error handling: The code does not properly handle error conditions, which can result in crashes and information leaks.

These vulnerabilities make this code unsafe to use in production systems.

This is a simple socket-based chat server written in C++ that uses the select system call to handle multiple clients. The server listens for incoming connections on port 8888 and accepts up to 30 clients.

When a client connects, the server sends a welcome message that outlines how to interact with the chat. Clients can send messages or execute commands in the format "--option: string". The server reads the input and executes it if it's a command or sends the message to all connected clients if it's a message. The server uses macros from sys/time.h (FD_SET, FD_ISSET, and FD_ZERO) to manage file descriptors. The server's main loop listens for client messages and uses the "notify_all" function to broadcast messages to all connected clients.

Currently this project is under construction, the main objective is to create a secure server and that all processes between server and client are secure.

This server is currently NetCat (nc) compatible.
To test, simply compile the project and run the executable.
The client will be able to use NetCat following the IP and port (8888) as simple as that.
