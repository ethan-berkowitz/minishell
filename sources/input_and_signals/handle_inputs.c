/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:11:12 by eberkowi          #+#    #+#             */
/*   Updated: 2024/11/18 15:54:42 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_inputs(char **input)
{
	activate_readline_signals();
	*input = readline("minishell: ");
	if (*input == NULL)
	{
		printf("exit\n");
		//print_error("exit\n");
		return (-1);
	}
	else if (*input && *input[0] == '\0')
		return (0);
	add_history(*input);
	return (1);
}
