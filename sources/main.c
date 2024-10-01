/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:12:47 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/01 13:21:11 by maheleni         ###   ########.fr       */
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
}

//Does $VAR need to expand to an array of strings or just a string?
//bash is able to execute something stored in a $, which implies that it's read as an array of strings
//But if that's the case, wouldn't I need to perform a split parsing on the var itself similar to the
//input split?

//Maybe expand the VAR/heredoc in the split process! then you dont have to remalloc

int	main(int argc, char *argv[], char *envp[]) //what happens if ./minishell gets arguments?
{	
	t_main 	main;
	t_tokens *tokens;		//why is tokens array not in main struct?

	(void)argc;
	(void)*argv;
	initialize_variables(&main, &tokens);
	copy_env(envp, &main);
	// update_env(&main);	//do we do this at all?
	//create_signals();
	while (1)
	{
		handle_inputs(&main.input);
		parsing(&main, &tokens);
		execute_commandline(&main, tokens);
		//set_exit_status_of_last_line();
	}
	if (main.input)
		free(main.input);
	free_environment(&(main.env_list));
	//free_signals();
	exit (main.exit_code);
}

int	main()
{
	struct stat	file_stat;

	if(stat("outfile", &file_stat) == 0)
	{
		printf("Success\n");
	}
	else
	{
		printf("Fail\n");
	}
}