/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:14:02 by dajesus-          #+#    #+#             */
/*   Updated: 2025/08/19 22:17:43 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *dup_until_quote(char *input, int *i, char quote)
{
    char *word;
    int   start;

    start = *i;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    word = ft_substr(input, start, *i - start);
    if (input[*i] == quote)
        (*i)++;
    return (word);
}

static t_word_info handle_quote_extraction(char *input, int *i,
    char *result, char *first_quote)
{
    char        *word;
    char        *temp;
    char        quote;
    t_word_info info;

    quote = input[*i];
    if (*first_quote == '\0')
        *first_quote = quote;
    (*i)++;
    word = dup_until_quote(input, i, quote);
    if (!word)
    {
        free(result);
        info.content = NULL;
        info.quote_type = '\0';
        return (info);
    }
    temp = result;
    result = ft_strjoin(result, word);
    free(temp);
    free(word);
    info.content = result;
    info.quote_type = *first_quote;
    return (info);
}

static t_word_info handle_char_extraction(char *input, int *i, char *result)
{
    char        *word;
    char        *temp;
    t_word_info info;

    word = malloc(2);
    if (!word)
    {
        free(result);
        info.content = NULL;
        info.quote_type = '\0';
        return (info);
    }
    word[0] = input[*i];
    word[1] = '\0';
    (*i)++;
    temp = result;
    result = ft_strjoin(result, word);
    free(temp);
    free(word);
    info.content = result;
    return (info);
}

static t_word_info process_character(char *input, int *i, char *result, char *first_quote)
{
    t_word_info info;

    if (is_quote(input[*i]))
        info = handle_quote_extraction(input, i, result, first_quote);
    else
        info = handle_char_extraction(input, i, result);
    return (info);
}

t_word_info extract_word_with_quotes(char *input, int *i)
{
    char        *result;
    char        first_quote;
    t_word_info info;

    result = ft_strdup("");
    first_quote = '\0';
    while (should_continue_extraction(input, *i))
    {
        info = process_character(input, i, result, &first_quote);
        if (!info.content)
            return (info);
        result = info.content;
    }
    info.content = result;
    info.quote_type = first_quote;
    return (info);
}
