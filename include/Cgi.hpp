/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: javier <javier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:04:39 by javier            #+#    #+#             */
/*   Updated: 2024/07/23 23:55:22 by javier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>  

extern char **g_envp;
int process_script(const std::string& script_path);

#endif