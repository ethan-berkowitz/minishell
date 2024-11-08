/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:45:33 by maheleni          #+#    #+#             */
/*   Updated: 2024/11/08 10:33:45 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_path_to_executable(char *command)
{
	if (access((const char *)command, X_OK) == 0)
		return (1);
	return (0);
}

int	is_path_to_file(char *command)
{
	if (access((const char *)command, F_OK) == 0
		&& ft_strchr(command, '/') != NULL)
		return (1);
	else if (ft_strchr(command, '/'))
		return (1);
	else
		return (0);
}

int	is_directory(char *command)
{
	struct stat	file_stat;

	if (access(command, F_OK) == 0)
	{
		if(stat(command, &file_stat) == 0)
		{
			if(S_ISDIR(file_stat.st_mode) && ft_strrchr(command, '/') != NULL)
			{
				errno = EISDIR;
				return (1);
			}
			return (0);
		}
		else
			return (1);
	}
	return (0);
}

char	*get_path(t_main *main, char *command, int *pids)
{
	errno = 0;
	if (is_directory(command))
		return (NULL);
	else if (is_path_to_file(command))
	{
		if (is_path_to_executable(command))
			return (ft_strdup(command));
		else
			return (NULL);
	}
	return (find_path(main, command, pids));
}
