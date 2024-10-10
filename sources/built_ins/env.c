/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:49:12 by maheleni          #+#    #+#             */
/*   Updated: 2024/10/10 13:54:22 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int env(t_main *main, t_tokens token)
{
    if (token.command[1] != NULL)
    {
        printf("Env does not take any arguments\n");
        return(1);
    }
    print_linked_list(main->env_list);
    return (0);
}
