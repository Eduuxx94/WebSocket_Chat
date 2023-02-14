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

Socket - summary

Here is the summary of key concepts:

    Socket is a way of speaking to other programs using standard file descriptors.
    Where do we get the file descriptor for network communication?
    Well, we make a call to the socket() system routine.
    After the socket() returns the socket descriptor, we start communicate through it using the specialized send()/recv() socket API calls.
    A TCP socket is an endpoint instance
    A TCP socket is not a connection, it is the endpoint of a specific connection.
    A TCP connection is defined by two endpoints aka sockets.
    The purpose of ports is to differentiate multiple endpoints on a given network address.
    The port numbers are encoded in the transport protocol packet header, and they can be readily interpreted not only by the sending and receiving computers, but also by other components of the networking infrastructure. In particular, firewalls are commonly configured to differentiate between packets based on their source or destination port numbers as in port forwarding.
    It is the socket pair (the 4-tuple consisting of the client IP address, client port number, server IP address, and server port number) that specifies the two endpoints that uniquely identifies each TCP connection in an internet.
    Only one process may bind to a specific IP address and port combination using the same transport protocol. Otherwise, we'll have port conflicts, where multiple programs attempt to bind to the same port numbers on the same IP address using the same protocol.


