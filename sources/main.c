/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:12:47 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/10 15:18:54 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initialize_variables(t_main *main, t_tokens **tokens)
{
	main->input = NULL;
	main->split_input = NULL;
	main->exit_code = 0;
	*tokens = NULL;
	main->env_list = NULL;
	main->num_of_pipes = 0;
	main->found_command = 0;
}

int	main(int argc, char *argv[], char *envp[]) //what happens if ./minishell gets arguments?
{	
	t_main 	main;
	t_tokens *tokens;

	(void)argc;
	(void)*argv;
	initialize_variables(&main, &tokens);
	copy_env(envp, &main);
	// update_env(&main);	//do we do this at all?
	// create_signals();
	while (1)
	{
		if (!handle_inputs(&main.input))
			continue;
		if (parsing(&main, &tokens))
			continue;
		execute_commandline(&main, tokens);
		//set_exit_status_of_last_line();
		free_and_null_split_input(&main);
		free_token_commands(&main, &tokens);
		free_token_redirects(&main, &tokens);
		free(tokens);
	}
	if (main.input) //REMOVE?
		free(main.input);
	free_environment(&(main.env_list));
	//free_signals();
	exit (main.exit_code);
}
