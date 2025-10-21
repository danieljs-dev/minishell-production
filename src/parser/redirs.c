/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:12 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:17 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

static t_redir_type	redir_type_from_token(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else
		return (REDIR_HEREDOC);
}

static void	handle_one_redir(t_token **tokens, t_redir **redirs)
{
	t_redir_type	type;
	t_redir			*node;

	type = redir_type_from_token((*tokens)->type);
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
		node = create_redir(type, ft_strdup((*tokens)->value));
	else
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		node = create_redir(type, ft_strdup(""));
	}
	if (node)
		add_redir(redirs, node);
	if (*tokens)
		*tokens = (*tokens)->next;
}

t_redir	*build_redirs(t_token *tokens)
{
	t_redir	*redirs;

	redirs = NULL;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redir_token(tokens->type))
			handle_one_redir(&tokens, &redirs);
		else
			tokens = tokens->next;
	}
	return (redirs);
}

int	has_redir_syntax_error(t_command *cmds)
{
	t_command	*c;
	t_redir		*r;

	c = cmds;
	while (c)
	{
		r = c->redirs;
		while (r)
		{
			if (!r->file || r->file[0] == '\0')
				return (1);
			r = r->next;
		}
		c = c->next;
	}
	return (0);
}
