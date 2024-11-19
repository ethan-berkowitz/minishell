/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:26:40 by maheleni          #+#    #+#             */
/*   Updated: 2024/11/18 14:29:32 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_signal_received = 0;

void	handle_sigint_readline(int sig)
{
	g_signal_received = 128 + sig;
	write(1, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
}

void	handle_sigint_heredoc(int sig)
{
	g_signal_received = 128 + sig;
	write(1, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
}