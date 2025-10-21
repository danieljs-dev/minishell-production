/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:38 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *search_in_paths(char **paths, char *cmd)
{
    int     i;
    char    *full_path;

    i = 0;
    while (paths[i])
    {
        full_path = join_path(paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_array(paths);
    return (NULL);
}

char *find_command_path(char *cmd, t_shell *shell)
{
    char    *path_env;
    char    **paths;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    path_env = get_env_value("PATH", shell);
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    free(path_env);
    if (!paths)
        return (NULL);
    return (search_in_paths(paths, cmd));
}
