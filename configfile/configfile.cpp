/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:54:45 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 09:02:43 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

void	check_syntax(std::string config_file)
{
    check_brackets(config_file);
}

void    start_parse_config_file(std::string config_file)
{
	check_syntax(config_file);
}
