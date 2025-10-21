/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:36:55 by dajesus-          #+#    #+#             */
/*   Updated: 2025/08/19 22:17:43 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void handle_redirection_token(t_token **tokens, char *input, int *i)
{
    if (input[*i] == '<' && input[*i + 1] == '<')
    {
        add_token(tokens, create_token(TOKEN_HEREDOC, NULL));
        *i += 2;
    }
    else if (input[*i] == '>' && input[*i + 1] == '>')
    {
        add_token(tokens, create_token(TOKEN_REDIR_APPEND, NULL));
        *i += 2;
    }
    else if (input[*i] == '<')
    {
        add_token(tokens, create_token(TOKEN_REDIR_IN, NULL));
        (*i)++;
    }
    else
    {
        add_token(tokens, create_token(TOKEN_REDIR_OUT, NULL));
        (*i)++;
    }
}

static void handle_word_token(t_token **tokens, char *input, int *i)
{
    t_word_info word_info;

    word_info = extract_word_with_quotes(input, i);
    if (word_info.content)
        add_token(tokens, create_quoted_token(TOKEN_WORD,
            word_info.content, word_info.quote_type));
}

t_token *tokenize(char *input)
{
    t_token *tokens;
    int     i;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        if (is_space(input[i]))
        {
            i++;
            continue ;
        }
        if (is_redir(input[i]))
            handle_redirection_token(&tokens, input, &i);
        else if (is_pipe(input[i]))
        {
            add_token(&tokens, create_token(TOKEN_PIPE, NULL));
            i++;
        }
        else
            handle_word_token(&tokens, input, &i);
    }
    return (tokens);
}
