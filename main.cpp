/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:35:36 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/26 12:26:56 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "./configfile/configfile.hpp"
#include "./server/socket.hpp"

std::string		read_file(std::string file_name)
{
	std::string	data;
	std::string	tmp;

	std::ifstream	file(file_name);
	while (getline(file, tmp))
	{
		data += tmp;
		data += "\n";
	}
	return data;
}

int main(int argc, char **argv)
{
	if (argc > 2)
		return (std::cout << "INVALID ARGUMENTS" << std::endl, 1);
	std::string config_file = (argc == 2 ? argv[1] : "./tests/def.conf");

	ConfigFile config;
	std::vector<Socket> sockets;
	std::vector<pfd>	pfds;
	std::map<int, ConnectSocket>    Connections;

	config_file = read_file(config_file);
	config = start_parse_config_file(config_file);
	// print_servers(config._servers);
	try
	{
		sockets = create_sockets(config);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
	listenSocket(sockets);
	pfds = create_pfd(sockets);
	while (1)
	{
		poll(&pfds[0], pfds.size(), -1);
		for (size_t i = 0; i < pfds.size(); i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				std::cout << "Pollin\n";
				pollin(pfds, sockets, Connections, i);
			}
			
			if (pfds[i].revents & POLLOUT && Connections[pfds[i].fd].SendAvailble)
			{
				std::cout << "Pollout\n";
				pollout(pfds, Connections, i);
			}

			if (pfds[i].revents & (POLLERR | POLLHUP))
			{
				std::cout << "PollError\n";
				close(pfds[i].fd);
				pfds.erase(pfds.begin() + i);
				i--;
			}
		}
	}
	return (0);
}
