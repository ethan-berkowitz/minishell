/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:21:19 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 15:19:00 by eberkowi         ###   ########.fr       */
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

#define INFILE 100
#define HEREDOC 101
#define OUTFILE 102
#define APPEND 103


/**
 * char		*input;
 * char		**split_input;
 * t_list	*env_list;
 * int		exit_code;
 */
typedef struct s_main
{
	char	*input;
	char	**split_input;
	t_list	*env_list;
	int		exit_code;  //exit code of the exit command given by user?
	int 	num_of_pipes;
	int		found_command;
} t_main;

typedef struct s_redirect_node t_redirect_node;

struct s_redirect_node
{
	char *name;
	int type;
	t_redirect_node *next;
};

typedef struct s_tokens
{
	char	**command;
	char	**heredoc_delimiter;
	t_redirect_node *infiles;
	t_redirect_node *outfiles;
}	t_tokens;

/*****************************************************************************/
	//INPUT AND SIGNALS
/*****************************************************************************/

//display prompt, readline, and save it in history
int	handle_inputs(char **input);

//error and exit for failed malloc in readline
int	error_exit_handle_input(void);

/*****************************************************************************/
	//PARSING
/*****************************************************************************/

//Master parsing function that calls are other functions for parsing
int	parsing(t_main *main, t_tokens **commands);

//A split for minishell copyright 2024
int	split_input(t_main *main);

//Free all previous malloced splits and exit
void	exit_free_split_element_malloc_failed(t_main *main, int i);

//Split the input into individual elements and check for no spaces before/after symbols
void	add_elements_to_split_input(t_main *main, char *input);

//Returns 1 if the given char is a special character that we need to handle, else 0
int	is_special(char c);

//Add single quote elements to the split_input array
void add_single_quotes_element(t_main *main, char *input, int *id_input, int id_split);

//Add double quote elements to the split_input array
void add_double_quotes_element(t_main *main, char *input, int *id_input, int id_split);

//Add special character elements to the split_input array
void add_redirect_element(t_main *main, char *input, int *i, int split_index);

//Checks for an exit command and exit-code and exits if found or shows error for incorrect format
void	exit_command(t_main *main);

//Our own kind of tokenizing function of the input
int tokenize(t_main *main, t_tokens **command);

//Utility to check for redirection (< or >)
int	is_redirect(char c);

//Free command struct, free split_input, and exit with the given code
void free_spl_and_cmd(t_main *main, t_tokens **command);

//Free the command_token utilizing ft_free_split
void free_token_commands(t_main *main, t_tokens **command);

//Free command tokens, command struct, and split_input, then exit with given code
void free_all_and_exit(t_main *main, t_tokens **command, int code);

//Checks for a syntax error where there are two redirects in a row or a redirect then NULL
int check_for_redirect_error(t_main *main, t_tokens **command);

//Checks for and adds redirect_in and heredocs to the token struct
void add_in_or_heredoc(t_main *main, t_tokens **command, int cmd_id, int *spl_id);

//Checks for and adds redirect_out and redirect_append to the token struct
void add_out_or_append(t_main *main, t_tokens **command, int cmd_id, int *spl_id);

//Free and null the input string (the basic input from the first readline)
void	free_and_null_input(t_main *main);

//Free and null the split_input
void	free_and_null_split_input(t_main *main);

//Checks for ' or "
int	is_quote(char c);

//Add command to token struct
void add_command(t_main *main, t_tokens **command, int cmd_id, int *spl_id);

//Expand the environment variables
void expand_variables(t_main *main);

//Utilities for creating and adding elements to a linked list
t_redirect_node	*lstnew_redirect_node(char *name, int type);
t_redirect_node	*lstlast_redirect_node(t_redirect_node *lst);
void lstadd_back_redirect_node(t_redirect_node **lst, t_redirect_node *new);

//Free the redirect linked lists of the tokens
void free_token_redirects(t_main *main, t_tokens **tokens);

//Free everything if a linked list node fails to malloc
void free_and_exit_node_malloc_failed(t_main *main, t_tokens **tokens);

/*****************************************************************************/
	//ENVIRONMENT
/*****************************************************************************/

/**
 * Frees all nodes an their content in the environment linked list
 * 
 * @param env_list pointer to the linked list to free
 */
void    free_environment(t_list **env_list);

/**
 * Finds and returns a node with the same key as variable
 * 
 * @param main the main struct of the program
 * @param variable a key-value pair in form KEY=value to find node for
 * @returns pointer to a node with the same key as variable
 */
t_list	*find_node(t_main *main, char *variable);

/**
 * Adds a variable to the environment (a new node to the linked list)
 * 
 * @param main pointer to the main struct
 * @param content the content of the node
 */
void    add_variable(t_main *main, char *content);

/**
 * Updates the value of the variable key in the env linked list
 * 
 * @param main pointer to the main struct
 * @param var the key value pair to update
 */
void	update_variable(t_main *main, char *var);

/**
 * Duplicates (mallocs) the values from envp into a linked list
 * 
 * @param envp the environment pointer gotten from main
 * @param main the main struct of the program
 */
t_list	*copy_env(char *envp[], t_main *main);

/**
 * Prints the contents of all nodes of the linked list
 * 
 * @param env_list the linked list to print
 */
void	print_linked_list(t_list *env_list);

/**
 * Helper function for print_linked_list function
 */
void	print_list_content(void *content);

/**
 * Frees and removes the node that has key varaible_key from the linked list
 * 
 * @param main the main struct of the program
 * @param variable_key the key of the variable to remove
 * @note variable_key needs to have '=' sign! E.g. "PATH="
 */
void	remove_variable(t_main *main, char *variable_key);

/*****************************************************************************/
	//BUILTINS
/*****************************************************************************/

/**
 * Prints the words after echo. If flag -n is given, does not print the newline
 * after last word.
 * 
 * @param main the main struct of the program
 * @param token the token that echo command is part of
 * @returns 0 on success, 1 on error
 */
int    echo(t_main *main, t_tokens token);

/**
 * Checks if the given key is already in the env list.
 * 
 * @param main the main struct of the program
 * @param var a variable in form VAR=value
 * @returns 1 when key was found, 0 when not found
 */
int		existing_key(t_main *main, char *var);

/**
 * Checks if the key of the variable to export has forbidden characters or in
 * forbidden form. I.e. a key kan only contain digits, letters and '_'. Digits
 * can not be in the beginning.
 * 
 * @param var the variable in form VAR=value
 * @returns 1 if key is forbidden, 0 if key is ok
 */
int		forbidden_key(char *var);

/**
 * Exports a variable of form VAR=value to the env list
 * 
 * @param main the main struct of the program
 * @param token the token to be executed
 */
void	export(t_main *main, t_tokens token);

/**
 * Unsets a variable from the env list.
 * 
 * @param main the main struct of the program
 * @param token the token to be executed
 */
void	unset(t_main *main, t_tokens token);

/**
 * Prints the current working directory
 * 
 * @param main the main struct of the program
 * @param token the token that pwd command is part of
 * @returns 0 on success, 1 on error
 */
int	pwd(t_main *main, t_tokens token);

/**
 * Prints the updated environment variables
 * 
 * @param main the main struct of the program
 * @param token the token that env command is part of
 * @returns 0 on success, 1 on error
 */
int env(t_main *main, t_tokens token);

/*****************************************************************************/
	//EXECUTION
/*****************************************************************************/

/**
 * Executes one commandline, by creating the necessary pipes and forking into
 * childprocesses when applicable. 
 * 
 * @param main the main struct of the program
 * @param tokens array of tokens to execute
 * @returns the exitstatus of the last executable
 */
int	execute_commandline(t_main *main, t_tokens *tokens);

/**
 * Closes the necessary pipe-filedescriptors depending on the executed commands
 * position in the command line.
 * 
 * @param i index of the command executed in commandline
 * @param num_of_pipes number of pipes left in commandline
 * @param pipe_left array of filedescriptors for the left-hand side pipe of command
 * @param pipe_right array of filedescriptors for the right-hand side pipe of command
 */
void	close_pipes_in_parent(int i, int num_of_pipes, int *pipe_left, int *pipe_right);

/**
 * Finds path to the command and execues it with execve
 * 
 * @param main the main struct of the program
 * @param token  the token to execute
 */
void	execute_command(t_main *main, t_tokens token);

int	execute_builtin(t_main *main, t_tokens token);

char	*find_path(t_main *main, char *command);

char	*get_path_variable(t_main *main);

char	**get_split_paths(char *path_variable);

int	set_path_if_executable(char *env_path, char *command, char **command_path);

char	*get_path(t_main *main, char **command);

int	empty_command(char *command);

int	is_direcotory(char *command);

int	is_path_to_executable(char *command);

int	is_path_to_file(char *command);

#endif
