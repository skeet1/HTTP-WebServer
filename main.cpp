/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:35:36 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/08 10:33:29 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "./configfile/configfile.hpp"
#include "./server/socket.hpp"
#include "./request/INCLUDES/request.hpp"

void set_error_pages(ConfigFile &config)
{
	std::map<std::string , std::string> error_pages;
	error_pages["404"] = "";
	error_pages[""] = "";
	//todo
}

void	start_server(std::string & _config)
{
	ConfigFile						configFile;
	std::vector<Socket>				sockets;
	std::vector<pfd>				pfds;
	std::map<int, ConnectSocket>	Connections;
	try
	{
		_config = read_file(_config);
		configFile = start_parse_config_file(_config);
		set_error_pages(configFile);
		sockets = create_sockets(configFile);
		listenSocket(sockets);
		pfds = create_pfd(sockets);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		poll(&pfds[0], pfds.size(), 0);
		for (size_t i = 0; i < pfds.size(); i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (i < sockets.size() && pfds[i].fd == sockets[i].getSocketId())
					pollin(pfds, sockets, Connections, i);
				else
				{
					Connections[pfds[i].fd].timeOut = getTimeOfNow();
					Connections[pfds[i].fd].readRequest(configFile);
					if (Connections[pfds[i].fd].closed)
					{
						closeConnection(pfds, Connections, i);
						i--;
					}
				}
			}
			if (pfds[i].revents & POLLOUT)
			{
				if (getTimeOfNow() - Connections[pfds[i].fd].timeOut >= 6)
				{
					std::cout << "There is A Time Out\n";
					closeConnection(pfds, Connections, i);
				}
				pollout(configFile, pfds, Connections, i);
				if (Connections[pfds[i].fd].closed || Connections[pfds[i].fd].conType)
				{
            		closeConnection(pfds, Connections, i);
					i--;
				}
			}
			if (pfds[i].revents & (POLLERR | POLLHUP))
			{
				pollErrHup(pfds, Connections, i);
				i--;
			}
		}
	}
}

int main(int argc, char **argv)
{
	if (argc > 2)
		return (std::cout << "INVALID ARGUMENTS" << std::endl, 1);
	std::string config_file = (argc == 2 ? argv[1] : "./tests/def.conf");
	start_server(config_file);	
	return (0);
}
