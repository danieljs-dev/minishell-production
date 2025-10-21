/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:24 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:39:42 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_env_var(t_shell *shell, int index)
{
	free(shell->env[index]);
	while (shell->env[index])
	{
		shell->env[index] = shell->env[index + 1];
		index++;
	}
}

static void	unset_single_var(char *name, t_shell *shell)
{
	int	j;
	int	len;

	len = ft_strlen(name);
	j = 0;
	while (shell->env[j])
	{
		if (ft_strncmp(shell->env[j], name, len) == 0
			&& shell->env[j][len] == '=')
		{
			remove_env_var(shell, j);
			break ;
		}
		j++;
	}
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_single_var(args[i], shell);
		i++;
	}
	return (0);
}
