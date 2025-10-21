/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:57 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *get_env_value(char *name, t_shell *shell)
{
    int     i;
    int     name_len;
    char    *env_var;

    if (!name || !shell->env)
        return (NULL);
    name_len = ft_strlen(name);
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, name_len) == 0
            && shell->env[i][name_len] == '=')
        {
            env_var = ft_strdup(&shell->env[i][name_len + 1]);
            return (env_var);
        }
        i++;
    }
    return (NULL);
}

char *expand_exit_status(char *input, t_shell *shell)
{
    char *result;

    if (!input)
        return (NULL);
    if (ft_strcmp(input, "$?") == 0)
    {
        result = ft_itoa(shell->exit_status);
        return (result);
    }
    return (ft_strdup(input));
}

static char *get_var_value_safe(char *var_name, t_shell *shell)
{
    char *var_value;

    var_value = get_env_value(var_name, shell);
    if (!var_value)
        var_value = ft_strdup("");
    return (var_value);
}

static char *join_prefix_and_value(char *prefix, char *var_value)
{
    char *temp;

    temp = ft_strjoin(prefix, var_value);
    free(prefix);
    free(var_value);
    return (temp);
}

char *expand_env_var(char *prefix, char *start, char *end, t_shell *shell)
{
    char    *var_name;
    char    *var_value;
    char    *temp;
    char    *result;

    var_name = ft_substr(start + 1, 0, end - start - 1);
    if (!var_name)
    {
        free(prefix);
        return (NULL);
    }
    var_value = get_var_value_safe(var_name, shell);
    free(var_name);
    if (!var_value)
    {
        free(prefix);
        return (NULL);
    }
    temp = join_prefix_and_value(prefix, var_value);
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, end);
    free(temp);
    return (result);
}
