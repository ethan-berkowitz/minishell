/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:32:36 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 15:10:13 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_split_input(t_main *main) //REMOVE
{
	printf("\033[0;33m---SPLIT_INPUT---\033[0m\n");
	int i = 0;
	while (main->split_input[i])
	{
		printf("input[%d] = %s\n", i, main->split_input[i]);
		i++;
	}
	printf("\n");
}

static void print_tokens(t_main *main, t_tokens **tokens) //REMOVE
{
	int token_number = 0;
	int i = 0;
	int j = 0;
	t_redirect_node *temp;

	while (token_number < main->num_of_pipes + 1)
	{
		printf("\033[0;32m---COMMAND %d---\033[0m\n", token_number);
		i = 0;
		if ((*tokens)[token_number].command)
		{
			while ((*tokens)[token_number].command[i])
			{
				printf("\033[0;31mcommand[%d] = %s\033[0m\n", i, (*tokens)[token_number].command[i]);
				i++;
			}
		}
		if ((*tokens)[token_number].heredoc_delimiter)
			printf("heredoc_delimiter = %s\n", *((*tokens)[token_number].heredoc_delimiter));
		

		
		//PRINT INFILES AND HEREDOCS
		j = 0;
		temp = (*tokens)[token_number].infiles;
		while (temp)
		{
			printf("infile[%d] name = %s, ", j, temp->name);
			if (temp->type == 100)
				printf("type = INFILE\n");
			else 
				printf("type = HEREDOC\n");
			temp = temp->next;
			j++;
		}
		
		//PRINT OUTFILES AND APPENDS
		j = 0;
		temp = (*tokens)[token_number].outfiles;
		while (temp)
		{
			printf("outfile[%d] name = %s, ", j, temp->name);
			if (temp->type == 102)
				printf("type = OUTFILE\n");
			else 
				printf("type = APPEND\n");
			temp = temp->next;
			j++;
		}

		token_number++;
		printf("\n");
	}
}

static void get_number_of_pipes(t_main *main)
{
	int i;

	i = 0;
	main->num_of_pipes = 0;
	while (main->split_input[i])
		if (main->split_input[i++][0] == '|')
			(main->num_of_pipes)++;
}

static void	malloc_commands(t_main *main, t_tokens **tokens, int size)
{
	*tokens = malloc((size) * sizeof(t_tokens));
	if (!*tokens)
	{
		ft_free_split(&main->split_input);
		printf("Error: Failed to malloc\n");
		exit (1);
	}	
}

static void	initialize_commands(t_tokens **tokens, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		(*tokens)[i].command = NULL;
		(*tokens)[i].heredoc_delimiter = NULL;
		(*tokens)[i].infiles = NULL;
		(*tokens)[i].outfiles = NULL;
		i++;
	}
}

static void malloc_and_init_tokens(t_main *main, t_tokens **tokens)
{
	get_number_of_pipes(main);
	malloc_commands(main, tokens, main->num_of_pipes + 1);
	initialize_commands(tokens, main->num_of_pipes + 1);
}

int	parsing(t_main *main, t_tokens **tokens)
{
	if (!split_input(main))
		return (1);
	print_split_input(main); //REMOVE
	free_and_null_input(main);
	exit_command(main);
	expand_variables(main);
	malloc_and_init_tokens(main, tokens);
	
	if (tokenize(main, tokens))
		return (1);
	
	print_tokens(main, tokens); //REMOVE
	return (0);
}
