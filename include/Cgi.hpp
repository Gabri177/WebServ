/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: javier <javier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:04:39 by javier            #+#    #+#             */
/*   Updated: 2024/07/24 19:38:12 by javier           ###   ########.fr       */
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
#include <cstring>
#include <sys/stat.h>

extern char **g_envp;
int process_script(const std::string& script_path);
int check_py_extension(const char* filename);

#endif