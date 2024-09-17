/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:32:36 by eberkowi          #+#    #+#             */
/*   Updated: 2024/09/17 19:42:34 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parsing(t_main *main)
{
	//main functions
	exit_for_testing(main); //REMOVE
	if (!split_input(main))
		return (0);

	//print for testing
	int i = 0;
	while (main->split_input[i])
	{
		printf("input[%d] = %s\n", i, main->split_input[i]);
		i++;
	}
	
	//free and null
	if (*main->input)
	{
		free(main->input);
		main->input = NULL;
	}	
	ft_free_split(&main->split_input);
	main->split_input = NULL;
	
	return (1);
}
