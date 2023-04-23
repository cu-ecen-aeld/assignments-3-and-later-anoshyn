#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    return (system(cmd) == 0) ? true : false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    
    pid_t pid = fork();
    if (pid == -1) {
    	perror("fork");
    	return false;
    } else if (pid == 0) {
    	// child process
    	execv(command[0], command);
    	perror("execv");
    	exit(EXIT_FAILURE);
    } else {
    	// parent process
    	int status;
    	if (waitpid(pid, &status, 0) == -1) {
    		perror("waitpid");
    		return false;
    	}
    	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    		return false;
    	}
    }

    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    // Open the output file for writing (truncating if it already exists)
    int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
    	perror("open");
    	return false;
    }
    
    // Fork the current process
    pid_t pid = fork();
    if (pid == -1) {
    	perror("fork");
    	close(fd);
    	return false;
    }
    
    if (pid == 0) {
    	// Child process
    	// Redirect standard output to the oupput file
    	if (dup2(fd, STDOUT_FILENO) == -1) {
    		perror("dup2");
    		close(fd);
    		_exit(EXIT_FAILURE);
    	}
    	
    	//Execute the command with execv
    	if (execv(command[0], command) == -1) {
    		perror("execv");
    		close(fd);
    		_exit(EXIT_FAILURE);
    	}
    } else {
    	// Parent process
    	// Wait for the child process to complete
    	int status;
    	if (waitpid(pid, &status, 0) == -1) {
    		perror("close");
    		return false;
    	}
    	
    	// Check if the command completed successfully
    	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    		return false;
    	}
    }

    va_end(args);

    return true;
}
