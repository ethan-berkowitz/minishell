/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:21:19 by eberkowi          #+#    #+#             */
/*   Updated: 2024/12/04 16:30:50 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>
# include <limits.h>

# define INFILE 100
# define HEREDOC 101
# define OUTFILE 102
# define APPEND 103
# define NULL_VAR 104

extern int		g_signal_received;

typedef struct s_redirect_node
{
	char					*name;
	char					*delimiter;
	bool					delimiter_has_quotes;
	int						type;
	struct s_redirect_node	*next;
}	t_redirect_node;

typedef struct s_tokens
{
	char			**command;
	t_redirect_node	*redirects;
}	t_tokens;

typedef struct s_expand
{
	bool	is_heredoc;
	char	quote_type;
}	t_expand;

typedef struct s_main
{
	char		*input;
	char		**split_input;
	t_list		*env_list;
	t_tokens	**tokens;
	long		exit_code;
	int			num_of_pipes;
	int			elements_in_command;
	int			id_command;
	int			quote_split_length;
	int			id_quote_split;
	int			num_of_existing_elements;
	int			rl_return;
	int			fd;
	int			pipes;
}	t_main;

typedef struct s_parsing
{
	int		id_char;
	int		num_of_singles;
	int		num_of_doubles;
	bool	inside_singles;
	bool	inside_doubles;
}	t_parsing;

/*****************************************************************************/
/*****************************************************************************/

	//MAIN

/*****************************************************************************/
/*****************************************************************************/

//Print error message to STD error
int				print_error(char *error);

void			remove_heredocs(t_main *main, t_tokens **tokens);

void			free_input_and_split(t_main *main);

/*****************************************************************************/
/*****************************************************************************/

	//INPUT AND SIGNALS

/*****************************************************************************/
/*****************************************************************************/

//display prompt, readline, and save it in history
int				handle_inputs(char **input, t_main *main);

void			activate_readline_signals(void);

void			activate_heredoc_signals(void);

void			activate_signals_for_child(void);

void			handle_sigint_readline(int sig);

void			handle_sigint_heredoc(int sig);

void			ignore_sigint(void);

/*****************************************************************************/
/*****************************************************************************/

	//PARSING

/*****************************************************************************/
/*****************************************************************************/

//Master parsing function that calls are other functions for parsing
int				parsing(t_main *main, t_tokens **commands);

//A split for minishell copyright 2024
int				split_input(t_main *main);

//Free all previous malloced splits and exit
void			exit_free_split_element_malloc_failed(t_main *main, int i);

//Split the input into individual elements and check for no spaces
int				add_elements_to_split_input(t_main *main, char *input);

//Returns 1 if the given char is a special character that we need to handle
int				char_is_special(char c);

//Add special character elements to the split_input array
void			add_special_element(t_main *main, char *input,
					int *i, int split_index);

//Our own kind of tokenizing function of the input
void			tokenize(t_main *main, t_tokens **tokens);

//Utility to check for redirection (< or >)
int				is_redirect(char c);

//Free the command_token utilizing ft_free_split
void			free_token_commands(t_main *main, t_tokens **tokens);

//Free command tokens, command struct, and split, then exit with given code
void			free_all_and_exit(t_main *main, t_tokens **tokens);

//Checks for syntax error where two redirects in a row or a redirect then NULL
int				check_for_redirect_error(t_main *main);

//Checks for and adds redirect_in and heredocs to the token struct
void			add_in_or_heredoc(t_main *main, t_tokens **tokens,
					int cmd_id, int *spl_id);

//Checks for and adds redirect_out and redirect_append to the token struct
void			add_out_or_append(t_main *main, t_tokens **tokens,
					int cmd_id, int *spl_id);

//Free and null the split_input
void			free_and_null_split_input(t_main *main);

//Checks for ' or "
int				is_quote(char c);

//Add command to token struct
void			add_command(t_main *main, t_tokens **tokens,
					int cmd_id, int *spl_id);

//Utilities for creating and adding elements to a linked list
t_redirect_node	*lstnew_redirect_node(char *name, int type);
t_redirect_node	*lstlast_redirect_node(t_redirect_node *lst);
void			lstadd_back_redirect_node(t_redirect_node **lst,
					t_redirect_node *new);

//Free the redirect linked lists of the tokens
void			free_token_redirects(t_main *main, t_tokens **tokens);

//Free everything if a linked list node fails to malloc
void			free_and_exit_node_malloc_failed(t_main *main,
					t_tokens **tokens);

//Create and readline the heredoc fd's and put them in the tokens
int				create_heredoc(t_main *main, t_tokens **tokens);

//Remove or interpret quotes and expand variables
void			quotes_and_variables(t_main *main, t_tokens **tokens);

//Free all commands above the NULL and free all and exit
void			free_and_exit_quote_malloc(t_main *main, t_tokens **tokens,
					int token_id, int cmd_id);

//Find the given $VAR in the env replace the given element if found
int				find_var_in_env(t_main *main, char **str);

//Free all and exit and free the split_input when there is a NULL in the middle
void			free_all_and_exit_with_free_split_middle(t_main *main,
					t_tokens **tokens);

//Print malloc error and free quote_split
int				print_error_and_free_split(int len, char ***quote_split);

//Removes outer double quotes and expands the environment vars within
int				expand_quotes_and_vars(t_main *main, t_tokens **tokens,
					char **str, bool is_heredoc);

//Free and  exit when malloc fails in combine elements for quote_split
void			free_and_exit_combine_elements(t_main *main,
					t_tokens **tokens, char ***quote_split);

//Frees the quote_split using the length of the malloced quote_split instead
//of the regular free_split because some elements are NULL
void			free_quote_split(int len, char ***arr);

//Expansion of quote and vars in the middle of words or double quotes
int				inner_expansion(t_main *main, char **str, bool is_heredoc);

//Remove the outer quotes of the given string
int				remove_outside_quotes(char **str);

//Create an array of strs that splits the elements of a str into var and quotes
int				create_quote_split(char *str, char ***quote_split);

//Check for single or double quotes on the outside of the given string
int				check_for_outside_quotes(char *str, char *quote_type);

//Add the given quote type to the outside of the given string
int				add_quotes_back_to_str(char **str, char quote_type);

//Returns len of one element of the quote_split for expanding quotes and vars
int				get_split_element_len(char *str, int i);

//Add elements to the quote split in quote and variable expansion
void			add_element_to_quote_split(char ***quote_split, char *str,
					int id_split, int id_str);

//Expand vars in the quote_split/do 'inner_expanion'expanding quotes and vars
void			expand_vars_or_do_inner_expansion(t_main *main,
					t_tokens **tokens, char ***quote_split, t_expand expand);

//Free and exit for malloc fail in expand_vars_or_do_inner_expansion
void			free_and_exit_quote_split_expand(t_main *main,
					t_tokens **tokens, char ***quote_split, int i);

//Check for adding back quotes to heredoc strs if removed during expansion
int				check_for_heredoc_quotes(char **str, bool is_heredoc,
					char quote_type, char ***quote_split);

//Skip spaces and tabs in given string starting at given index
void			skip_spaces_and_tabs(char *input, int *id_input);

//Check if given char is a space,tab, special char and if the quotes are closed
int				char_is_space_or_special_and_quotes_are_closed(char c,
					t_parsing p);

//Update the 'inside status' of the given struct
void			update_inside_status(t_parsing *p);

//Update the number of the given quote for the given struct
void			update_number_of_quotes(char c, t_parsing *p);

//Check for unclosed quotes in initial input parsing
int				check_for_unclosed_quotes(t_parsing p);

//Checks for a pipe error where there is a pipe then nothing following
int				check_for_pipe_error(t_main *main, t_tokens **tokens);

//Return and change the index of the next non-NULL element in quote_split
int				next_id(t_main *main, char **quote_split);

//Free tokens and print error but NOT exit for pipe syntax error
int				pipe_syntax_error(t_main *main, t_tokens **tokens);

//Free all that need to be freed in order to continue from main loop
int				free_all_for_heredoc(t_main *main, t_tokens **tokens);

//Readline to heredoc and check for signals that may interupt
int				readline_to_file(t_main *main, t_tokens **tokens,
					t_redirect_node *temp);

//Hook for signals
int				event(void);

//Combines the quote_split after everything has been expanded
int				combine_quote_split(t_main *main, t_tokens **tokens,
					char ***quote_split, char **str);

//Check for malloc fail or received signal in create_heredoc
int				check_malloc_fail_or_signal(t_main *main, t_tokens **tokens,
					int heredoc_fd, char *input);

//Receive a signal in create_heredoc
int				handle_signal_received(t_main *main, t_tokens **tokens,
					int heredoc_fd, char **input);

/*****************************************************************************/
/*****************************************************************************/

	//ENVIRONMENT

/*****************************************************************************/
/*****************************************************************************/

/**
 * Frees all nodes an their content in the environment linked list
 * 
 * @param env_list pointer to the linked list to free
 */
void			free_environment(t_list **env_list);

/**
 * Finds and returns a node with the same key as variable
 * 
 * @param main the main struct of the program
 * @param variable a key-value pair in form KEY=value to find node for
 * @returns pointer to a node with the same key as variable
 */
t_list			*find_node(t_main *main, char *variable);

/**
 * Adds a variable to the environment (a new node to the linked list)
 * 
 * @param main pointer to the main struct
 * @param content the content of the node
 */
int				add_variable(t_main *main, char *content);

/**
 * Updates the value of the variable key in the env linked list
 * 
 * @param main pointer to the main struct
 * @param var the key value pair to update
 */
int				update_variable(t_main *main, char *var);

/**
 * Duplicates (mallocs) the values from envp into a linked list
 * 
 * @param envp the environment pointer gotten from main
 * @param main the main struct of the program
 */
t_list			*copy_env(char *envp[], t_main *main);

/**
 * Prints the contents of all nodes of the linked list
 * 
 * @param env_list the linked list to print
 */
void			print_linked_list(t_list *env_list);

/**
 * Helper function for print_linked_list function
 */
void			print_list_content(void *content);

/**
 * Frees and removes the node that has key varaible_key from the linked list
 * 
 * @param main the main struct of the program
 * @param variable_key the key of the variable to remove
 */
void			remove_variable(t_main *main, char *variable_key);

char			**convert_list_to_array(t_list *env_list);

/*****************************************************************************/
/*****************************************************************************/

	//BUILTINS

/*****************************************************************************/
/*****************************************************************************/

/**
 * Changes the working directory of the shell. Also updates the PWD and OLDWPD
 * variable in the env list.
 * 
 * @param main pointer to the main struct of the program
 * @param token the token that cd command is part of
 * @returns 
 */
int				cd(t_main *main, t_tokens token);

/**
 * Gets the path to home directory from env list.
 * 
 * @param main pointer to the main struct of the program
 * @returns The path as a string, NULL if HOME variable is unset
 */
char			*get_path_to_home(t_main *main);

/**
 * Updates the PWD and OLDPWD variables in env list depending on the key
 * 
 * @param main pointer to the main struct of the program
 * @param key the key of the variable to update (only PWD or OLDPWD)
 */
int				update_directory_variable(t_main *main, char *key);

/**
 * Prints the words after echo. If flag -n is given, does not print the newline
 * after last word.
 * 
 * @param main the main struct of the program
 * @param token the token that echo command is part of
 * @returns 0 on success, 1 on error
 */
int				echo(t_main *main, t_tokens token);

/**
 * Checks for a newline flag on the given string (-n or -nnnnn etc.)
 * 
 * @param str the string that we are checking for newline flag
 * @returns 1 on true, 0 on false
 */
int				check_for_newline_flag(char *str);

/**
 * Prints the arguments
 * 
 * @param token the token that echo command is part of
 */
void			print_echo_arguments(t_tokens token);

/**
 * Prints the current environment variables and applies any newline flags
 * 
 * @param token the token that env command is part of
 */
int				env(t_main *main, t_tokens token);

/**
 * Exits the program with the given value, 0 if no argument is given
 * 
 * @param main the main struct of the program
 * @param token the token that exit command is part of
 * @param parent 1 if we are in parent process, 0 if in child process
 * @param open_fds array of the open file descriptors
 * @returns the exitcode if in child process
 */
long			exit_command(t_main *main, t_tokens token, int parent,
					int open_fds[2]);

/**
 * If in child process: Returns with exit_code.
 * If in parent process: Frees and closes everyhitng and exits.
 * 
 * @param main the main struct of the program
 * @param open_fds array of the open file descriptors
 * @param exit_code the code to exit with or return
 */
void			free_and_exit(t_main *main, int open_fds[2], int exit_code);

/**
 * Sets the numeric value given with the command into temp_code.
 * 
 * @param element the element that can be the numeric value
 * @param exit_code pointer to variable to hold the exit code in
 * @returns 1 if numeric value, 0 if not
 */
int				int_after_exit(char *element, long *exit_code);

/**
 * Exports a variable of form VAR or VAR=value to the env list
 * 
 * @param main the main struct of the program
 * @param token the token to be executed
 * @returns 0 on success, 1 on error
 */
int				export(t_main *main, t_tokens token);

/**
 * Prints error message and the variable that is refers to
 * 
 * @param argument the key-value pair that is to be printed
 * @returns 1, that will be set to the return value of export command
 */
int				print_forbidden_key(char *argument);

/**
 * Checks if the key of the variable to export has forbidden characters or in
 * forbidden form. I.e. a key can only contain digits, letters and '_'. Digits
 * can not be in the beginning.
 * 
 * @param var the variable in form VAR=value
 * @returns 1 if key is forbidden, 0 if key is ok
 */
int				forbidden_key(char *var);

/**
 * Checks if the given key is already in the env list.
 * 
 * @param main the main struct of the program
 * @param var a variable in form VAR=value
 * @returns 1 when key was found, 0 when not found
 */
int				existing_key(t_main *main, char *var);

/**
 * Prints all exported variables in ASCII order
 * 
 * @param main the main struct of the program
 * @returns 0 on success, 1 on fail
 */
int				export_without_args(t_main *main);

/**
 * Finds the node that has the contents next in ascii order
 * 
 * @param prev_node pointer to the previously printed node
 * @param biggest_node pointer to the node that will be printed last
 * @param env_list pointer to the env_list (export_list) that will be printed
 * @returns the node to print the contents of next
 */
t_list			*get_next_node(t_list *prev_node, t_list *biggest_node,
					t_list *env_list);

/**
 * Finds and returns the node that will be printed last
 * 
 * @param env_list pointer to the env_list (export_list) that will be printed
 * @returns the node that will be printed last
 */
t_list			*get_biggest_node(t_list *env_list);

/**
 * Finds and returns the node that will be printed first
 * 
 * @param env_list pointer to the env_list (export_list) that will be printed
 * @returns the node that will be printed first
 */
t_list			*get_smallest_node(t_list *env_list);

/**
 * Prints the current working directory
 * 
 * @param main the main struct of the program
 * @param token the token that pwd command is part of
 * @returns 0 on success, 1 on error
 */
int				pwd(t_main *main, t_tokens token);

/**
 * Returns the value of PWD variable if it exists
 * 
 * @param main the main struct of the program
 * @returns value of PWD or NULL
 */
char			*get_pwd_from_env(t_main *main);

/**
 * Gets and returns the current working directory in a buffer that is malloced
 * by getcwd()
 * 
 * @returns the path to the current working direcotry
 */
char			*get_pwd(void);

/**
 * Removes a variable from the env list (export list)
 * 
 * @param main the main struct of the program
 * @param token the token to be executed
 * @returns 0 on success, 1 on error
 */
int				unset(t_main *main, t_tokens token);

//Itoa but for longs for exit code handling
char			*ft_itoa_long(long n);

/*****************************************************************************/
/*****************************************************************************/

	//EXECUTION

/*****************************************************************************/
/*****************************************************************************/

/**
 * Executes one commandline, by creating the necessary pipes and forking into
 * childprocesses when applicable. 
 * 
 * @param main the main struct of the program
 * @param tokens array of tokens to execute
 */
void			execute_commandline(t_main *main, t_tokens *tokens);

/**
 * Allocates the needed amount of memory for the process ids
 * 
 * @param main the main struct of the program
 * @param amount the amount of processes to run in that pipeline
 * (number of pipes + 1)
 * @returns pointer to the memory allocated
 */
int				*malloc_pids(t_main *main, int amount);

/**
 * Loops through all commands in the pipeline, creates the needed pipes and
 * forks to run the commands in a childprocess, handles redirects and
 * runs the command
 * 
 * @param main the main struct of the program
 * @param pipe_array the array of filedescriptors for the pipe(s)
 * @param pids allocated array of process ids
 * @param tokens the array of tokens to loop through
 * @returns 0 on success, > 0 when some system functioncalls in parent failed
 */
int				execute_pipeline(t_main *main, int pipe_array[2][2], int *pids,
					t_tokens *tokens);

/**
 * Checks if command is a builtin or not
 * 
 * @param the token that command is part of
 * @returns 1 if command is builtin, 0 if not
 */
int				is_builtin(t_tokens token);

/**
 * Finds path to the command and execues it with execve
 * 
 * @param main the main struct of the program
 * @param token  the token to execute
 */
void			execute_command(t_main *main, t_tokens token, int *pids);

/**
 * Executes either a builtin or a command as a child process.
 * If a builtin is executed, the process is exited with the return
 * value of the builtin-function.
 * 
 * @param main the main struct of the program
 * @param token  the token to execute
 */
void			execute_child_process(t_main *main, t_tokens token, int *pids,
					int pipe_right[2]);

/**
 * Executes a bultin in the parent process. Redirects STDIN and STDOUT
 * to correct files if necessary. After execution restores STDIN and SDTOUT
 * filedescriptors.
 * 
 * @param main the main struct of the program
 * @param token the token to execute
 * @param num_of_pipes the number of pipes left in pipeline
 */
int				execute_builtin_in_parent(t_main *main, t_tokens token,
					int num_of_pipes);

/**
 * Checks if the command is a builtin and that it is not part of a pipeline
 * 
 * @param token the token to check
 * @param num_of_pipes the number of pipes in pipeline
 * @returns 1 when command is a builtin not part of pipeline, 0 otherwise
 */
int				is_builtin_not_part_of_pipeline(t_tokens token,
					int num_of_pipes);

/**
 * Restores the filedescriptors to point back to STDIN and STDOUT
 * 
 * @param main the main struct of the program
 * @param original_stdin duped fd of STDIN
 * @param original_stdout duped fd of STDOUT
 */
void			restore_stdin_stdout(t_main *main, int original_stdin,
					int original_stdout);

/**
 * Calls the right builtin function to execute it
 * 
 * @param main the main struct of the program
 * @param token  the token to execute
 */
int				execute_builtin(t_main *main, t_tokens token, int parent,
					int open_fds[2]);

/**
 * Finds the path to the command by looking up the PATH variable
 * 
 * @param main the main struct of the program
 * @param command the command to find the path for
 * @param pids allocated array of processids
 * @returns the path to the command, NULL if path was not found
 */
char			*find_path(t_main *main, char *command, int *pids);

/**
 * Returns the PATH variables value as a (malloced) array of strings
 * 
 * @param main the main struct of the program
 * @param pids allocated array of processids
 * @returns array of paths, NULL if PATH is unset
 */
char			**get_split_paths(t_main *main, int *pids);

/**
 * Finds and returns the value of PATH variable
 * 
 * @param main the main struct of the program
 * @returns value of PATH variable, NULL if PATH is unset
 */
char			*get_path_variable(t_main *main);

/**
 * Sets the path to the command into command_path if it is a path to an
 * executable
 * 
 * @param env_path one of the paths from PATH variable
 * @param command the command to check if is executable in that path
 * @param command_path the variable where to set the path to the
 * executable into
 * @returns 1 on success, 0 on error
 */
int				set_path_if_executable(char *env_path, char *command,
					char **command_path);

/**
 * Returns a path to the command executable
 * When
 * 	a) command is a relative or absolute path
 * 	-> returns an allocated copy of that command
 * 	b) command is just a word
 * 	-> tries to find the path to the executable with the name of command
 * 		from the PATH variable, when successfull returns path, NULL otherwise
 * 
 * @param main the main struct of the program
 * @param command the command to get path to
 * @param pids allocated array of processids
 * @returns the path to the command, the command or NULL
 */
char			*get_path(t_main *main, char *command, int *pids);

/**
 * Checks if the PATH variable exist in the environment
 * 
 * @param main the main struct of the program
 * @returns 1 when PATH does not exist, 0 when it exists
 */
int				path_does_not_exist(t_main *main);

/**
 * Checks if the command begins with a dot or contains a slash, i.e. if the
 * command is a path
 * 
 * @param command the command to check
 * @returns 1 when it contains either . or /, 0 if dos not contain either
 */
int				command_begins_with_dot_or_contains_slash(char *command);

/**
 * Checks if command is a directory
 * 
 * @param command the command to check
 * @returns 1 when directory, 0 when not
 */
int				is_directory(char *command);

/**
 * Loops through all redirects and opens them. Does dup2 only for the
 * last opened infile and outfile
 * 
 * @param token the token that the rediects are part of
 * @returns 0 on success, -1 on fail
 */
int				handle_redirects(t_tokens token);

/**
 * Does dup2 for the infile and outfile
 * 
 * @param infile filedescriptor for the last infile for this command
 * @param outfile filedescriptor for the last outfile for this command
 * @returns 0 on success, -1 on fail
 */
int				dup2_redirects(int infile, int outfile);

/**
 * Opens the file that node holds
 * 
 * @param infile pointer to the filedescriptor of the last opened infile
 * @param outfile pointer to the filedescriptor of the last opened outfile
 * @param node pointer to the redirect node that holds the filename to open
 * @returns 0 on success, -1 on fail
 */
int				open_file(int *infile, int *outfile, t_redirect_node *node);

/**
 * Opens outfile
 * 
 * @param node pointer to the redirect node that holds the filename to open
 * @param infile pointer to the infile filedescriptor
 * @returns 0 on success, -1 on fail
 */
int				open_outfile(t_redirect_node *node, int *infile);

/**
 * Opens infile
 * 
 * @param node pointer to the redirect node that holds the filename to open
 * @returns 0 on success, -1 on fail
 */
int				open_infile(t_redirect_node	*node);

/**
 * Prepares the pipes for the next comand
 * 
 * @param i the index of the command to prepare
 * @param num_of_pipes the number of pipes left in pipeline
 * @param pipe_array the array that holds the filedescriptors for left and
 * right pipes
 * @param pids array of processids
 * @returns 1 on success, -1 on error
 */
int				prepare_pipes(int i, int num_of_pipes, int pipe_array[2][2],
					int *pids);

/**
 * Reassigns the right-hand side pipes filedescriptors (i.e. pipe_array[1]) to
 * the left-hand side (i.e. pipe_array[0])
 * 
 * @param pipe_array the array that holds the filedescriptors for left and
 * right pipes
 */
void			reassign_pipe_right_to_left(int pipe_array[2][2]);

/**
 * Forks a process
 * 
 * @param i the index of the command
 * @param num_of_pipes the number of pipes left in pipeline
 * @param pipe_array the array that holds the filedescriptors for left and
 * right pipes
 * @param pids array of processids
 * @returns the processid of that process on success, -1 on error
 */
int				create_fork(int i, int num_of_pipes, int pipe_array[2][2],
					int *pids);

/**
 * Closes both sides of a pipe
 * 
 * @param array of both filedescriptors for that pipe
 */
void			close_pipes_on_error(int *pipe);

/**
 * Closes the necessary pipe-filedescriptors depending on the executed commands
 * position in the command line.
 * 
 * @param i index of the command executed in commandline
 * @param num_of_pipes number of pipes left in commandline
 * @param pipe_left array of fds for the left-hand side pipe of command
 * @param pipe_right array of fds for the right-hand side pipe of command
 */
void			close_pipes_in_parent(int i, int num_of_pipes, int *pipe_left,
					int *pipe_right);

/**
 * Does dup2 for the created pipes
 * 
 * @param i index of the command executed in commandline
 * @param num_of_pipes number of pipes left in commandline
 * @param pipe_array the array that holds the filedescriptors for left and
 * right pipes
 * @param token the token that these pipes are part of
 * @returns 0 on success, -1 on error
 */
int				redirect_pipes(int i, int num_of_pipes, int pipe_array[2][2],
					t_tokens token);

/**
 * Does dup2 on the right-hand side pipe
 * 
 * @param pipe_right array of filedescriptors for the right-hand side pipe
 * @returns 0 on success, -1 on error
 */
int				redirect_pipe_right(int *pipe_right, t_tokens token);

/**
 * Does dup2 on the left-hand side pipe
 * 
 * @param pipe_right array of filedescriptors for the left-hand side pipe
 * @returns 0 on success, -1 on error
 */
int				redirect_pipe_left(int *pipe_left, t_tokens token);

/**
 * Frees everything that is allocated and the array of processids
 * 
 * @param main the main struct of the program
 * @param pids array of processids
 */
void			free_all_in_child(t_main *main, int *pids);

/**
 * Frees everything that is mmalloced in the parent process
 * 
 * @param main the main struct of the program
 */
void			free_all_in_parent(t_main *main);

/**
 * Writes the correct error and exits when problem with getting path to cmd
 * 
 * @param main the main struct of the program
 * @param token the token that the error has occured in
 * @param pids array of processids
 */
void			path_error_handling(t_main *main, t_tokens token, int *pids);

#endif
