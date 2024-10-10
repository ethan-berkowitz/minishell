/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:11:12 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 14:00:45 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Left off here, newlineand new prompt when no prompt!
//It's the enviroment ! not fgreeing the env

static int	display_prompt_and_wait_for_input(char **input)
{
	*input = readline("minishell: ");
	if (*input && *input[0] == '\0')
		return (0);
	return (1);
}

int handle_inputs(char **input)
{
	if (!display_prompt_and_wait_for_input(input))
		return (0);
	if (*input)
		add_history(*input);
	return (1);
}
