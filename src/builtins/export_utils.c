/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:28:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 20:40:18 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cmp_env_keys_ascii(const char *a, const char *b)
{
	size_t	i;
	char	ca;
	char	cb;

	i = 0;
	while (1)
	{
		ca = a[i];
		cb = b[i];
		if (ca == '=')
			ca = '\0';
		if (cb == '=')
			cb = '\0';
		if (ca != cb || ca == '\0' || cb == '\0')
			return ((unsigned char)ca - (unsigned char)cb);
		i++;
	}
}

void	sort_env_ascii(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (cmp_env_keys_ascii(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	*escape_export_value(const char *value)
{
	size_t  i;
	size_t  k;
	char    *out;

	if (!value)
		return (ft_strdup(""));
	i = ft_strlen(value);
	out = (char *)malloc((i * 2) + 1);
	if (!out)
		return (NULL);
	k = 0;
	i = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\')
			out[k++] = '\\';
		out[k++] = value[i++];
	}
	out[k] = '\0';
	return (out);
}

int	env_append(t_shell *shell, char *new_env)
{
	int		n;
	char	**resized;

	n = array_size(shell->env);
	resized = (char **)malloc(sizeof(char *) * (n + 2));
	if (!resized)
		return (1);
	while (n-- > 0)
		resized[n] = shell->env[n];
	resized[array_size(shell->env)] = new_env;
	resized[array_size(shell->env) + 1] = NULL;
	free(shell->env);
	shell->env = resized;
	return (0);
}
