/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:35 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:55:21 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**copy_env(char **env)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			free_array(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	array_size(char **array)
{
	int	size;

	size = 0;
	if (!array)
		return (0);
	while (array[size])
		size++;
	return (size);
}

char	*join_path(char *path1, char *path2)
{
	char	*result;
	char	*temp;

	if (!path1 || !path2)
		return (NULL);
	temp = ft_strjoin(path1, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, path2);
	free(temp);
	return (result);
}
