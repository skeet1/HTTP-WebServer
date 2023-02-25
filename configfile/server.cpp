/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:39:08 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 16:28:36 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
	// std::cout << "Server Default Constructor" << std::endl;
	_client_max_body_size = 1;
}

Server::~Server()
{
	// std::cout << "Server Destrucotr" << std::endl;
}