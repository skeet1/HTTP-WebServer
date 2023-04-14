#include "../INCLUDES/request.hpp"


std::string conc(std::string s1, std::string s2)
{
    return s1 + '=' + s2;
}

static std::string  readFromPipe(int PipeId)
{
    int             readedChar;
    std::string     data;
    char            buffer[BUFFER] = {0};

    // readedChar = read(PipeId, buffer, BUFFER);
    while ((readedChar = read(PipeId, buffer, BUFFER)) > 0)
    {
        data.append(buffer, readedChar);
    }
    return (data);
}

void set_env(ConnectSocket &socket, location location,Server server, ConfigFile configfile, char **env)
{
    std::string server_name = socket.IpAdress + ":" + socket.Port;
    // if(socket._request.)
    std::string path_info = socket._request.original_request_target.substr(socket._request.original_request_target.find(".php") + 4,\
    (socket._request.original_request_target.find('?') ) - (socket._request.original_request_target.find(".php") + 4));
    std::string script_name = socket._request.request_target.substr(socket._request.request_target.find_last_of('/') + 1, socket._request.request_target.find(".php") + 4);
    std::string querystring;
    if(socket._request.original_request_target.find('?') != std::string::npos)
        querystring = socket._request.original_request_target.substr(socket._request.original_request_target.find('?') + 1);
    std::cout << "server_name: "<< server_name << std::endl;
    std::cout << "path_info: " << path_info << std::endl;
    std::cout << "script_name: " << script_name << std::endl;
    std::cout << "query_string: " << querystring << std::endl;

    env[0] = strdup("SERVER_SOFTWARE=NGINY/0.1");
    env[1] = strdup(conc("SERVER_NAME", server_name).c_str());
    env[2] = strdup(conc("SERVER_PROTOCOL", "HTTP/1.1").c_str());
    env[3] = strdup(conc("SERVER_PORT", socket.Port).c_str());
    env[4] = strdup(conc("REQUEST_METHOD", socket._request.method).c_str());
    if(socket._request.method == "POST")
    {
        env[5] = strdup(conc("CONTENT_TYPE", socket._request.headers_map["Content-type"]).c_str());
        env[6] = strdup(conc("CONTENT_LENGTH", socket._request.headers_map["Content-Length"]).c_str());
    }
    env[7] = strdup(conc("PATH_INFO", path_info).c_str());
    env[8] = strdup(conc("SCRIPT_NAME", script_name).c_str());
    env[9] = strdup(conc("QUERY_STRING", querystring).c_str());
}

void cgi_handler(ConnectSocket &socket, location location,Server server, ConfigFile configfile)
{
    int         pid;
    int         fds[2];
    std::string body;

    if (pipe(fds) < 0)
    {
        std::cout << "PIPE Error\n";
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        char *env[10];
        char *args[2];

        set_env(socket, location, server, configfile, env);
        args[0] = strdup("cgi-bin/php-cgi");
        args[0] = strdup(socket._request.request_target.c_str());
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        execve("cgi-bin/php-cgi", args, env);
    }
    // read data written from the pipe
    body = readFromPipe(fds[0]);
    std::cout << "body : "<< body << std::endl;
    close(fds[0]);
    close(fds[1]);
}