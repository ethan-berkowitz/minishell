/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 12:02:21 by eberkowi          #+#    #+#             */
/*   Updated: 2024/10/09 12:03:24 by eberkowi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_redirect_node	*lstnew_redirect_node(char *name, int type)
{
	t_redirect_node	*new_node;

	new_node = (t_redirect_node *)malloc(sizeof(t_redirect_node));
	if (!new_node)
		return (NULL);
	new_node->name = name;
	new_node->type = type;
	new_node->next = NULL;
	return (new_node);
}

t_redirect_node	*lstlast_redirect_node(t_redirect_node *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	lstadd_back_redirect_node(t_redirect_node **lst, t_redirect_node *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		lstlast_redirect_node(*lst)->next = new;
	else
		*lst = new;
}
