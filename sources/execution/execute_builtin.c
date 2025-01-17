/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:49:13 by maheleni          #+#    #+#             */
/*   Updated: 2024/12/05 14:20:57 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtin(t_main *main, t_tokens token, int parent, int open_fds[2])
{
	char	*command;

	command = token.command[0];
	if (ft_strncmp(command, "echo", ft_strlen(command)) == 0)
		return (echo(main, token));
	else if (ft_strncmp(command, "cd", ft_strlen(command)) == 0)
		return (cd(main, token));
	else if (ft_strncmp(command, "pwd", ft_strlen(command)) == 0)
		return (pwd(main, token));
	else if (ft_strncmp(command, "export", ft_strlen(command)) == 0)
		return (export(main, token));
	else if (ft_strncmp(command, "unset", ft_strlen(command)) == 0)
		return (unset(main, token));
	else if (ft_strncmp(command, "env", ft_strlen(command)) == 0)
		return (env(main, token));
	else if (ft_strncmp(command, "exit", ft_strlen(command)) == 0)
		return (exit_command(main, token, parent, open_fds));
	return (0);
}

void	restore_stdin_stdout(t_main *main, int original_stdin,
	int original_stdout)
{
	if (dup2(original_stdin, STDIN_FILENO) == -1
		|| dup2(original_stdout, STDOUT_FILENO) == -1)
	{
		close(original_stdin);
		close(original_stdout);
		perror("Exiting minishell because of");
		free_all_in_parent(main);
		exit(1);
	}
	close(original_stdin);
	close(original_stdout);
}

int	is_builtin_not_part_of_pipeline(t_tokens token, int num_of_pipes)
{
	if (token.command == NULL || token.command[0] == NULL)
		return (0);
	if (is_builtin(token) && num_of_pipes == 0)
		return (1);
	return (0);
}

int	execute_builtin_in_parent(t_main *main, t_tokens token, int num_of_pipes)
{
	int	orig_stdin_stdout[2];

	if (token.command[0] != NULL
		&& is_builtin_not_part_of_pipeline(token, num_of_pipes))
	{
		orig_stdin_stdout[0] = dup(STDIN_FILENO);
		orig_stdin_stdout[1] = dup(STDOUT_FILENO);
		if (orig_stdin_stdout[0] == -1 || orig_stdin_stdout[1] == -1)
		{
			perror(NULL);
			main->exit_code = errno;
			return (1);
		}
		if (handle_redirects(token) == -1)
		{
			restore_stdin_stdout(main, orig_stdin_stdout[0],
				orig_stdin_stdout[1]);
			main->exit_code = 1;
			return (1);
		}
		main->exit_code = execute_builtin(main, token, 1, orig_stdin_stdout);
		restore_stdin_stdout(main, orig_stdin_stdout[0], orig_stdin_stdout[1]);
		return (1);
	}
	return (0);
}
