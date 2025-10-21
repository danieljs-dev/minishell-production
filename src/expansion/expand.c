/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:04:03 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *expand_single_var(char *input, t_shell *shell)
{
    char    *start;
    char    *end;
    char    *prefix;

    start = ft_strchr(input, '$');
    if (!start)
        return (ft_strdup(input));
    end = start + 1;
    if (start[1] == '?')
        end = start + 2;
    else
    {
        while (*end && (*end == '_' || ft_isalnum(*end)))
            end++;
    }
    prefix = ft_substr(input, 0, start - input);
    if (!prefix)
        return (NULL);
    if (end - start == 2 && start[1] == '?')
        return (expand_exit_var(prefix, end, shell));
    else
        return (expand_env_var(prefix, start, end, shell));
}

char *expand_exit_var(char *prefix, char *end, t_shell *shell)
{
    char    *exit_str;
    char    *temp;
    char    *result;

    exit_str = ft_itoa(shell->exit_status);
    if (!exit_str)
    {
        free(prefix);
        return (NULL);
    }
    temp = ft_strjoin(prefix, exit_str);
    free(prefix);
    free(exit_str);
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, end);
    free(temp);
    return (result);
}

char *expand_variables_with_quotes(char *input, char quote_type, t_shell *shell)
{
    char    *result;
    char    *temp;
    char    *current;

    if (!input)
        return (NULL);
    if (quote_type == '\'')
        return (ft_strdup(input));
    if (ft_strlen(input) > 0 && input[ft_strlen(input) - 1] == '$')
        return (ft_strdup(input));
    result = ft_strdup(input);
    current = result;
    while (ft_strchr(current, '$'))
    {
        temp = expand_single_var(current, shell);
        free(current);
        current = temp;
        if (!current)
            return (NULL);
    }
    return (current);
}

static int should_skip_expansion(char *input)
{
    if (ft_strlen(input) > 0 && input[ft_strlen(input) - 1] == '$')
        return (1);
    if (ft_strchr(input, '*') && (ft_strchr(input, '"') || ft_strchr(input, '\'')))
        return (1);
    return (0);
}

char *expand_variables(char *input, t_shell *shell)
{
    char    *result;
    char    *temp;
    char    *current;

    if (!input)
        return (NULL);
    if (ft_strncmp(input, "__SINGLE_QUOTED__", 17) == 0)
        return (ft_strdup(input + 17));
    if (should_skip_expansion(input))
        return (ft_strdup(input));
    result = ft_strdup(input);
    current = result;
    while (ft_strchr(current, '$'))
    {
        temp = expand_single_var(current, shell);
        free(current);
        current = temp;
        if (!current)
            return (NULL);
    }
    return (current);
}
