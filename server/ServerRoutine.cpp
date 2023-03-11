#include "socket.hpp"
#include "ConnectSocket.hpp"


int main(int ac, char *av[])
{
    if (ac < 2)
    {
        std::cout << "Input Error: Usage <Host> <port> <port> ...." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::vector<Socket>             sockets;
    std::vector<pfd>                pfds;
    std::map<int, ConnectSocket>    Connections;
    int                             connection;
    sockStorage                     so_storage;
    pfd                             temp;

    for (int i = 2; i < ac; i++)
    {
        Socket sock(av[1], av[i]);
        sockets.push_back(sock);
    }
    for (unsigned int i = 0; i < sockets.size(); i++)
    {
        if (listen(sockets[i].getSocketId(), 50) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    for (unsigned int i = 0; i < sockets.size() ; i++)
    {
        temp.fd = sockets[i].getSocketId();
        temp.events = POLLIN | POLLOUT;
        pfds.push_back(temp);
    }
    socklen_t       socket_len;
    unsigned int    i;

    std::ifstream is ("index.html", std::ifstream::binary);
    is.seekg(0, is.end);
    int length =is.tellg();
    is.seekg(0, is.beg);
    char * buffer = new char [length];
    is.read(buffer, length);
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
    response.append(std::to_string(length));
    response.append("\r\n\r\n");
    response.append(std::string (buffer, buffer + length));
    
    i = 0;
    while (1)
    {
        poll(&pfds[0], pfds.size(), -1);
        i = 0;
        while (i < pfds.size())
        {
            if (pfds[i].revents & POLLIN)
            {
                if (i < sockets.size() && pfds[i].fd == sockets[i].getSocketId())
                {
                    connection = accept(pfds[i].fd, (sockaddr *)&so_storage, &socket_len);
                    temp.fd = connection;
                    temp.events = POLLIN | POLLOUT;
                    pfds.push_back(temp);
                    Connections[connection] = ConnectSocket(connection);
                    Connections[connection].setResponseLength(response.size());
                }
                else
                {
                    if (Connections.find(pfds[i].fd) != Connections.end() && Connections[pfds[i].fd].ReadAvailble)
                        Connections[pfds[i].fd].read_request();
                }
            }

            if (pfds[i].revents & POLLOUT && Connections[pfds[i].fd].SendAvailble)
            {
                if (Connections.find(pfds[i].fd) != Connections.end())
                {
                    Connections[pfds[i].fd].ConnectionType(Connections[pfds[i].fd].Request);
                    Connections[pfds[i].fd].send_response(response);
                    if (!Connections[pfds[i].fd].SendAvailble && Connections[pfds[i].fd].ConnectionClosed)
                    {
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin()+i);
                        i--;
                    }
                }
            }

            if (pfds[i].revents & (POLLERR | POLLHUP))
            {
                close(pfds[i].fd);
                pfds.erase(pfds.begin()+i);
                i--;
            }
            i++;
        }
    }
    return (0);
}