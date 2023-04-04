#include "../INCLUDES/request.hpp"
#include <dirent.h>

void file2response(ConnectSocket &socket, Server &server, location &location)
{
    socket._response.response_string = "tobe defined in file2response function";
}

int listdir(location location)
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir(location.path.c_str());
    if(!dir)
        return 0;
    while((ent = readdir(dir)))
        std::cout << ent->d_name << std::endl;
    closedir(dir);
    return 1;
}

void GET(ConnectSocket &socket, Server &server, location &location)
{
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(location._index.size())
            socket._request.request_target += location._index[0];
        else
            socket._request.request_target += server._index[0];
    }
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/' && (location._autoindex == "on" || (!location._autoindex.size() && server._autoindex == "on")))
    {
        listdir(location);
        return ;
    }
    else if (socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(opendir(socket._request.request_target.c_str()) != NULL)
            socket._response.response_string = respond_error("403");//responde with 403 forbidden and 
        else
            socket._response.response_string = respond_error("404");//responde with 404 notfound and
    }
    //check if the content is dynamic or static and server each one separatly
    // if(socket._request.request_target.size() >= 4 && strcmp(socket._request.request_target.c_str() + socket._request.request_target.size() - 4, ".php"))
        //cgi here; return the response from here; return
    
    //static here
    std::cout << socket._request.request_target << std::endl;
    if(!access(socket._request.request_target.c_str(), F_OK))
        file2response(socket, server, location);
    else 
        socket._response.response_string = respond_error("404");//404 not found 
}