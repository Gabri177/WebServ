/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabpalma <pabpalma>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:47:26 by pabpalma          #+#    #+#             */
/*   Updated: 2024/07/25 11:50:30 by pabpalma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/utils.cpp
#include "../include/Utils.hpp"

std::string removeDuplicateSlashes(const std::string& path) {
	std::string result;
	bool lastWasSlash = false;

	for (size_t i = 0; i < path.length(); ++i) {
		char currentChar = path[i];
		if (currentChar == '/') {
			if (!lastWasSlash) {
			    result += currentChar;
			    lastWasSlash = true;
			}
		} else {
			result += currentChar;
			lastWasSlash = false;
		}
	}
	
	return result;
}

