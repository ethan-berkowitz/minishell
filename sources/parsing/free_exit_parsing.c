/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:44:31 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 15:13:40 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_spl_and_cmd(t_main *main, t_tokens **tokens)
{
	ft_free_split(&main->split_input);
	free(*tokens);
}

void free_all_and_exit(t_main *main, t_tokens **tokens, int code)
{
	ft_free_split(&main->split_input);
	free_token_commands(main, tokens);
	free(*tokens);
	exit (code);
}

void	free_and_null_input(t_main *main)
{
	if (*main->input)
	{
		free(main->input);
		main->input = NULL;
	}
}

void	free_and_null_split_input(t_main *main)
{
	if (main->split_input)
	{
		ft_free_split(&main->split_input);
		main->split_input = NULL;
	}
}

void	exit_free_split_element_malloc_failed(t_main *main, int i)
{
	while (i >= 0)
	{
		free(main->split_input[i]);
		i--;
	}
	free(main->split_input);
	free(main->input);
	exit (1);
}

void	free_token_commands(t_main *main, t_tokens **tokens)
{
	int i;
	
	i = 0;
	while (i < main->num_of_pipes + 1)
	{
		if ((*tokens)[i].command)
			ft_free_split(&((*tokens)[i].command));
		i++;
	}
}

//Left off here trying to free linked list, test isn't printing right?

void free_token_redirects(t_main *main, t_tokens **tokens)
{
	int token_number;
	t_redirect_node *temp;
	t_redirect_node *temp_next;

	token_number = 0;
	while (token_number < main->num_of_pipes + 1)
	{
		temp = (*tokens)[token_number].infiles;
		while (temp)
		{
			temp_next = temp->next;
			free(temp);
			temp = temp_next;
		}
		temp = (*tokens)[token_number].outfiles;
		while (temp)
		{
			temp_next = temp->next;
			free(temp);
			temp = temp_next;
		}
		token_number++;
	}
}

void free_and_exit_node_malloc_failed(t_main *main, t_tokens **tokens)
{
	printf("Error: Failed to malloc node for redirect linked list\n");
	ft_free_split(&main->split_input);
	free_token_commands(main, tokens);
	free_token_redirects(main, tokens);
	free(*tokens);
	exit (1);
}
