/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:20 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:39:19 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_with_value(char *name, char *value, t_shell *shell)
{
	int		n;
	char	*new_env;
	char	*temp1;
	char	**resized;

	temp1 = ft_strjoin(name, "=");
	new_env = ft_strjoin(temp1, value);
	free(temp1);
	if (!new_env)
		return (0);
	builtin_unset((char *[]){"unset", name, NULL}, shell);
	n = array_size(shell->env);
	resized = (char **)malloc(sizeof(char *) * (n + 2));
	if (!resized)
	{
		free(new_env);
		return (1);
	}
	while (n-- > 0)
		resized[n] = shell->env[n];
	resized[array_size(shell->env)] = new_env;
	resized[array_size(shell->env) + 1] = NULL;
	free(shell->env);
	shell->env = resized;
	return (0);
}

static int	handle_export_with_value(char *name, char *value, t_shell *shell)
{
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	export_with_value(name, value, shell);
	return (0);
}

static int	handle_export_no_value(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	return (0);
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	int		result;

	value = ft_strchr(arg, '=');
	if (value)
	{
		*value = '\0';
		name = arg;
		value++;
		result = handle_export_with_value(name, value, shell);
		*(value - 1) = '=';
		return (result);
	}
	else
		return (handle_export_no_value(arg));
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (builtin_env(shell));
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], shell) != 0)
			return (1);
		i++;
	}
	return (0);
}
