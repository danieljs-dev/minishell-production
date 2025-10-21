/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:39:50 by dajesus-          #+#    #+#             */
/*   Updated: 2025/08/19 22:17:43 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->quote_type = '\0';
    token->next = NULL;
    return (token);
}

t_token *create_quoted_token(t_token_type type, char *value, char quote_type)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->quote_type = quote_type;
    token->next = NULL;
    return (token);
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *current;

    if (!*tokens)
    {
        *tokens = new_token;
        return ;
    }
    current = *tokens;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

void free_tokens(t_token **tokens)
{
    t_token *current;
    t_token *next;

    if (!tokens)
        return ;
    current = *tokens;
    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
    *tokens = NULL;
}
