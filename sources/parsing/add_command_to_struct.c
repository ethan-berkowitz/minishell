/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_command_to_struct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:37:36 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/09 16:09:08 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int count_elements_in_command(t_main *main, int spl_id)
{
	int result;

	result = 0;
	while (main->split_input[spl_id] && !is_special(main->split_input[spl_id][0]))
	{
		result++;
		spl_id++;
	}
	return (result);
}

static char	*strdup_with_remove_quotes(const char *s1)
{
	int		i;
	char	*s2;

	s2 = (char *)malloc((ft_strlen(s1) - 2) * sizeof(char) + 1);
	if (!s2)
		return (NULL);
	i = 0;
	while (s1[i + 1] && !is_quote(s1[i + 1]))
	{
		s2[i] = s1[i + 1];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

static void malloc_cmds(t_main *main, t_tokens **tokens, int cmd_id, int c)
{
	(*tokens)[cmd_id].command = malloc((c + 1) * sizeof(char *));
	if (!(*tokens)[cmd_id].command)
	{
		printf("Error: Failed to malloc command array in struct\n");
		free_all_and_exit(main, tokens, 1);
	}
}

void add_command(t_main *main, t_tokens **tokens, int cmd_id, int *spl_id)
{
	int element_count;
	int i;

	element_count = count_elements_in_command(main, *spl_id);
	if (element_count == 0)
		return ;
	else if (main->found_command == 1)
	{
		printf("Error: Too many commands\n");
		free_all_and_exit(main, tokens, 1);
	}
	main->found_command = 1;
	malloc_cmds(main, tokens, cmd_id, element_count);
	i = 0;
	while (i < element_count)
	{
		(*tokens)[cmd_id].command[i] = NULL;
		if (is_quote((main->split_input[*spl_id])[0]))
			(*tokens)[cmd_id].command[i] = strdup_with_remove_quotes(main->split_input[*spl_id]);
		else
			(*tokens)[cmd_id].command[i] = ft_strdup(main->split_input[*spl_id]);
		if (!(*tokens)[cmd_id].command[i])
		{
			printf("Error: Failed to malloc add element to command\n");
			free_all_and_exit(main, tokens, 1);
		}
		(*spl_id)++;
		i++;
	}
	(*tokens)[cmd_id].command[i] = NULL;
}
