/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:04:37 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *handle_quoted_arg(t_token *token)
{
    char *temp;

    if (token->quote_type == '\'')
    {
        temp = ft_strjoin("__SINGLE_QUOTED__", token->value);
        return (temp);
    }
    else
        return (ft_strdup(token->value));
}

int count_args(t_token *tokens)
{
    int count;

    count = 0;
    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_WORD)
            count++;
        else if (is_redir_token(tokens->type))
        {
            tokens = tokens->next;
            if (tokens && tokens->type == TOKEN_WORD)
                tokens = tokens->next;
            continue ;
        }
        tokens = tokens->next;
    }
    return (count);
}

static int append_word_arg(char **args, int i, t_token *token)
{
    args[i] = handle_quoted_arg(token);
    if (!args[i])
    {
        free_array(args);
        return (-1);
    }
    return (i + 1);
}

static t_token *skip_redir_pair(t_token *tokens)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
        tokens = tokens->next;
    return (tokens);
}

char **build_args(t_token *tokens, int count)
{
    char    **args;
    int     i;

    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);
    i = 0;
    while (i < count && tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_WORD)
        {
            i = append_word_arg(args, i, tokens);
            if (i == -1)
                return (NULL);
            tokens = tokens->next;
        }
        else if (is_redir_token(tokens->type))
            tokens = skip_redir_pair(tokens);
        else
            tokens = tokens->next;
    }
    args[i] = NULL;
    return (args);
}
