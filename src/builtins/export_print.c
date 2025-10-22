/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:29:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 20:40:18 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_no_value(char *entry)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putendl_fd(entry, 1);
}

static int	extract_kv(char *entry, char **key, char **val)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (0);
	*key = ft_substr(entry, 0, eq - entry);
	*val = ft_strdup(eq + 1);
	if (!*key || !*val)
		return (free(*key), free(*val), 1);
	return (1);
}

static int	print_kv(char *key, char *val)
{
	char	*escaped;

	escaped = escape_export_value(val);
	if (!escaped)
		return (1);
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(escaped, 1);
	ft_putendl_fd("\"", 1);
	free(escaped);
	return (0);
}

static int	print_one_export(char *entry)
{
	char	*key;
	char	*val;
	int		has_kv;
	int		res;

	key = NULL;
	val = NULL;
	has_kv = extract_kv(entry, &key, &val);
	if (!has_kv)
		return (print_no_value(entry), 0);
	res = print_kv(key, val);
	free(key);
	free(val);
	return (res);
}

int	print_export_list(t_shell *shell)
{
	char	**copy;
	int		i;

	copy = copy_env(shell->env);
	if (!copy)
		return (1);
	sort_env_ascii(copy);
	i = 0;
	while (copy[i])
	{
		if (print_one_export(copy[i]) != 0)
			return (free_array(copy), 1);
		i++;
	}
	free_array(copy);
	return (0);
}
