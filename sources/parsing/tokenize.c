/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:23:05 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 15:13:13 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int check_for_pipe_error(t_main *main, t_tokens **tokens)
{
	int i;

	i = 0;
	while (main->split_input[i])
	{
		if ((main->split_input[i])[0] == '|'
			&& !(main->split_input[i + 1]))
		{
				printf("Error: Pipe followed by NULL\n");
				free_spl_and_cmd(main, tokens);
				return (1);
		}
		i++;
	}
	return (0);
}

//Need to change redirect tokens to array of strings to hold multiple infiles/outfiles

int tokenize(t_main *main, t_tokens **tokens)
{
	int cmd_id;
	int spl_id;

	if (check_for_pipe_error(main, tokens))
		return (1);
	if (check_for_redirect_error(main, tokens))
		return (1);
	cmd_id = 0;
	spl_id = 0;
	
	while (cmd_id < main->num_of_pipes + 1)
	{
		add_in_or_heredoc(main, tokens, cmd_id, &spl_id);
		add_out_or_append(main, tokens, cmd_id, &spl_id);
		add_command(main, tokens, cmd_id, &spl_id);
		if (!main->split_input[spl_id] || main->split_input[spl_id][0] == '|')
		{
			cmd_id++;
			spl_id++;
			main->found_command = 0;
		}
	}
	return (0);
	//TODO function for expanding heredocs with a temp_file that holds the input
}
