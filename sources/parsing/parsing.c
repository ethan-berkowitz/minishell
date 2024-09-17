/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:32:36 by eberkowi          #+#    #+#             */
/*   Updated: 2024/09/17 11:22:42 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parsing(t_main *main)
{
	exit_for_testing(main); //REMOVE
	mini_split(main);

	int i = 0;
	while (main->split_input[i])
	{
		printf("index %d = %s\n", i, main->split_input[i]);
		i++;
	}
	
	if (*main->input)
	{
		free(main->input);
		main->input = NULL;
	}	
	ft_free_split(&main->split_input);
	main->split_input = NULL;
}
