/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:40:26 by dajesus-          #+#    #+#             */
/*   Updated: 2025/08/19 22:17:43 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_redir(char c)
{
    return (c == '<' || c == '>');
}

int is_pipe(char c)
{
    return (c == '|');
}

int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

int should_continue_extraction(char *input, int i)
{
    return (input[i] && !is_space(input[i]) && !is_redir(input[i])
        && !is_pipe(input[i]));
}
