/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:21:19 by eberkowi          #+#    #+#             */
/*   Updated: 2024/09/24 10:46:37 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

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
} t_main;

typedef struct s_command
{
	char	**command;
	char	**heredoc_delimiter;
	char	**redirect_in;
	char	**redirect_out;
	bool    redirect_heredoc;
	char	**redirect_append;
}	t_command;

/*****************************************************************************/
	//INPUT AND SIGNALS
/*****************************************************************************/

//display prompt, readline, and save it in history
void	handle_inputs(char **input);

//error and exit for failed malloc in readline
int	error_exit_handle_input(void);

/*****************************************************************************/
	//PARSING
/*****************************************************************************/

//Master parsing function that calls are other functions for parsing
int	parsing(t_main *main, t_command **commands);

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
void add_special_character_element(t_main *main, char *input, int *i, int split_index);

//Checks for an exit command and exit-code and exits if found or shows error for incorrect format
void	exit_command(t_main *main);

//Our own kind of tokenizing function of the input
void tokenize(t_main *main, t_command **command);

//Utility to check for redirection (< or >)
int	is_redirect(char c);

//Free command struct, free split_input, and exit with the given code
void free_and_exit_spl_and_cmd(t_main *main, t_command **command, int code);

/*****************************************************************************/
	//ENVIRONMENT
/*****************************************************************************/

/**
 * Frees all nodes in the environment linked list (but not the content, because
 * the content is malloced by split?)
 * 
 * @param env_list pointer to the linked list to free
 */
void    free_environment(t_list **env_list);

/**
 * Adds a variable to the environment (a new node to the linked list)
 * 
 * @param main pointer to the main struct
 * @param content the content of the node
 */
void    add_variable(t_main *main, char *content);

/**
 * Copies the values from envp into a linked list
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
 * Removes one node from the linked list
 * 
 * @param main the main struct of the program
 * @param variable_key the key of the variable to remove
 * @note variable_key needs to have '=' sign! E.g. "PATH="
 */
void	remove_variable(t_main *main, char *variable_key);

#endif