/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:04:46 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:10 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_commands(t_command **commands)
{
	t_command	*current;
	t_command	*next;

	if (!commands)
		return ;
	current = *commands;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_array(current->args);
		if (current->redirs)
			free_redirs(&current->redirs);
		free(current);
		current = next;
	}
	*commands = NULL;
}
