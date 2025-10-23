/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:55:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/23 12:54:28 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_has_key(t_shell *shell, const char *name)
{
	int	i;
	int	len;

	if (!shell || !shell->env || !name)
		return (0);
	len = ft_strlen(name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strcmp(shell->env[i], name) == 0)
			return (1);
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	env_remove_bare(t_shell *shell, const char *name)
{
	int	i;

	if (!shell || !shell->env || !name)
		return ;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strcmp(shell->env[i], name) == 0)
		{
			free(shell->env[i]);
			while (shell->env[i])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			return ;
		}
		i++;
	}
}
