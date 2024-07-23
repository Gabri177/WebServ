/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: javier <javier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 23:26:45 by javier            #+#    #+#             */
/*   Updated: 2024/07/24 00:06:38 by javier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*#define MAX_ENV_VARS 128  // Adjust as needed

// Function to handle script execution using fork and execve
int process_script(const std::string& script_path) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Error: fork failed." << std::endl;
        return 1; // Indicate error
    }

    if (pid == 0) { // Child process
        // Access environment variables from global g_envp (assuming proper setup)
        char* envp[MAX_ENV_VARS];
        for (int i = 0; g_envp[i] && i < MAX_ENV_VARS - 1; ++i) {
            envp[i] = g_envp[i];
        }
        envp[MAX_ENV_VARS - 1] = NULL; // Terminate environment

        // Execute the script using execve (with proper error handling)
char* argv[] = { const_cast<char*>(script_path.c_str()), NULL };
if (execve(script_path.c_str(), argv, envp) == -1) {
            perror("execve");
            exit(1); // Child process exit (indicates error)
        }
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 0; // Script execution successful
        } else {
            return 1; // Script execution failed
        }
    }
*/

#define MAX_ENV_VARS 128

int process_script(const std::string& script_path) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Error: fork failed." << std::endl;
        return 1;
    }

    if (pid == 0) {
        char* envp[MAX_ENV_VARS];
        for (int i = 0; g_envp[i] && i < MAX_ENV_VARS - 1; ++i) {
            envp[i] = g_envp[i];
        }
        envp[MAX_ENV_VARS - 1] = NULL;

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