/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:13:07 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/15 13:58:50 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_in_or_heredoc(t_main *main, t_tokens **tokens, int cmd_id,
	int *spl_id)
{
	t_redirect_node	*node;

	node = NULL;
	if (!ft_strncmp(main->split_input[*spl_id], "<", 2))
	{
		node = lstnew_redirect_node(main->split_input[*spl_id + 1], INFILE);
		if (!node)
			free_and_exit_node_malloc_failed(main, tokens);
		if (!(*tokens)[cmd_id].infiles)
			(*tokens)[cmd_id].infiles = node;
		else
			lstadd_back_redirect_node(&(*tokens)[cmd_id].infiles, node);
		(*spl_id) += 2;
	}
	else if (!ft_strncmp(main->split_input[*spl_id], "<<", 3))
	{
		node = lstnew_redirect_node(main->split_input[*spl_id + 1], HEREDOC);
		if (!node)
			free_and_exit_node_malloc_failed(main, tokens);
		if (!(*tokens)[cmd_id].infiles)
			(*tokens)[cmd_id].infiles = node;
		else
			lstadd_back_redirect_node(&(*tokens)[cmd_id].infiles, node);
		(*spl_id) += 2;
	}
}

void	add_out_or_append(t_main *main, t_tokens **tokens, int cmd_id,
	int *spl_id)
{
	t_redirect_node	*node;

	node = NULL;
	if (!ft_strncmp(main->split_input[*spl_id], ">", 2))
	{
		node = lstnew_redirect_node(main->split_input[*spl_id + 1], OUTFILE);
		if (!node)
			free_and_exit_node_malloc_failed(main, tokens);
		if (!(*tokens)[cmd_id].outfiles)
			(*tokens)[cmd_id].outfiles = node;
		else
			lstadd_back_redirect_node(&(*tokens)[cmd_id].outfiles, node);
		(*spl_id) += 2;
	}
	else if (!ft_strncmp(main->split_input[*spl_id], ">>", 3))
	{
		node = lstnew_redirect_node(main->split_input[*spl_id + 1], APPEND);
		if (!node)
			free_and_exit_node_malloc_failed(main, tokens);
		if (!(*tokens)[cmd_id].outfiles)
			(*tokens)[cmd_id].outfiles = node;
		else
			lstadd_back_redirect_node(&(*tokens)[cmd_id].outfiles, node);
		(*spl_id) += 2;
	}
}

int	check_for_redirect_error(t_main *main, t_tokens **tokens)
{
	int	i;

	i = 0;
	while (main->split_input[i])
	{
		if (is_redirect((main->split_input[i])[0]))
		{
			if (!(main->split_input[i + 1]))
			{
				printf("syntax error near unexpected token '\\n'\n");
				free_split_and_tokens(main, tokens);
				return (1);
			}
			if (is_redirect((main->split_input[i + 1])[0]))
			{
				printf("syntax error near unexpected token '%s'\n",
					main->split_input[i + 1]);
				free_split_and_tokens(main, tokens);
				return (1);
			}
		}
		i++;
	}
	return (0);
}
