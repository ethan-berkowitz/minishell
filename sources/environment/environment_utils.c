/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:37:05 by maheleni          #+#    #+#             */
/*   Updated: 2024/10/17 15:18:40 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    free_environment(t_list **env_list)
{
	t_list	*current_node;
	t_list	*temp;

	current_node = *env_list;
    while (current_node != NULL)
	{
		temp = current_node->next;
		free(current_node->content);
		free(current_node);
		current_node = temp;
	}
}

char	**convert_list_to_array(t_list *env_list)
{
	int		size;
	char	**array;
	t_list	*node;
	int		i;

	size = ft_lstsize(env_list);
	array = malloc ((size + 1) * sizeof(char *));
	if (array == NULL)
	{
		perror(NULL);
		return (NULL);
	}
	node = env_list;
	i = 0;
	while (node != NULL)
	{
		array[i] = node->content;
		i++;
		node = node->next;
	}
	array[i] = NULL;
	return (array);
}

t_list	*copy_env(char *envp[], t_main *main)
{
	t_list	*env_list;
	t_list	*new;
	int		i;

	new = ft_lstnew(ft_strdup(envp[0]));
	if (new == NULL)
	{
		perror("Error");
        ft_free_split(&(main->split_input));
	}
	env_list = new;
	main->env_list = env_list;
	i = 1;
	while (envp[i] != NULL)
	{
		add_variable(main, envp[i]);
		i++;
	}
	return (env_list);
}
