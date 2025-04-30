/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:03:53 by adesille          #+#    #+#             */
/*   Updated: 2025/04/30 13:04:09 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

std::string intToString(int n){
	std::ostringstream oss;
	oss << n;
	return oss.str();
}
