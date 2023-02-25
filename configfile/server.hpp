/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:53:21 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 16:30:46 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "location.hpp"

class Server
{
	public:
		std::map<std::string, std::vector<std::string> > 	_listen;
		std::string											_root;
		std::vector<std::string>							_server_names;
		std::vector<std::string>							_index;
		std::vector<location>								_locations;
		std::map<std::string, std::string>					_error_pages;
		std::string											_client_max_body_size;

	public:
		Server();
		~Server();
};