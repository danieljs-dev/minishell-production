/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:20 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/23 12:54:28 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_with_value(char *name, char *value, t_shell *shell)
{
	char	*new_env;
	char	*temp1;

	temp1 = ft_strjoin(name, "=");
	new_env = ft_strjoin(temp1, value);
	free(temp1);
	if (!new_env)
		return (0);
	env_remove_bare(shell, name);
	builtin_unset((char *[]){"unset", name, NULL}, shell);
	if (env_append(shell, new_env) != 0)
	{
		free(new_env);
		return (1);
	}
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

static int	handle_export_no_value(char *arg, t_shell *shell)
{
	char	*dup;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	if (!env_has_key(shell, arg))
	{
		dup = ft_strdup(arg);
		if (!dup)
			return (1);
		if (env_append(shell, dup) != 0)
		{
			free(dup);
			return (1);
		}
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
		return (handle_export_no_value(arg, shell));
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (print_export_list(shell));
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], shell) != 0)
			return (1);
		i++;
	}
	return (0);
}
