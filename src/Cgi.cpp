/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: javier <javier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 23:26:45 by javier            #+#    #+#             */
/*   Updated: 2024/07/24 20:06:53 by javier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#define MAX_ENV_VARS 128

int check_py_extension(const char* filename)
{
    const char* extension = strrchr(filename, '.'); 
    if (extension == NULL || strcmp(extension, ".py") != 0)
        return -1;
    return 0;
}

int process_script(const std::string& script_path) {
    // Set the script file to be executable
    if (chmod(script_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == -1) {
        std::cerr << "Error: chmod failed. " << strerror(errno) << std::endl;
        return 1;
    }

    // Sleep for a short time to avoid file being busy
    sleep(1);

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Error: fork failed." << std::endl;
        return 1;
    }

    if (pid == 0) {
        char* envp[MAX_ENV_VARS];
        int i;
        for (i = 0; g_envp[i] && i < MAX_ENV_VARS - 1; ++i) {
            envp[i] = g_envp[i];
        }
        envp[i] = NULL;

        char* argv[] = { const_cast<char*>(script_path.c_str()), NULL };

        if (execve(script_path.c_str(), argv, envp) == -1) {
            perror("execve");
            _exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 0;
        } else {
            return 1;
        }
    }
    return 1;
}