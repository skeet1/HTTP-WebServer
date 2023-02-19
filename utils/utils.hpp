/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 16:57:11 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 17:07:46 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

std::string					str_trim(std::string s);
std::vector<std::string>	str_split(std::string s, char c);

#endif