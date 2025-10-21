/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:04:50 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:14 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*commands;
	t_command	*cmd;
	char		**args;
	t_redir		*redirs;
	int			arg_count;

	commands = NULL;
	while (tokens)
	{
		arg_count = count_args(tokens);
		args = build_args(tokens, arg_count);
		redirs = build_redirs(tokens);
		cmd = create_command(args, redirs);
		if (cmd)
			add_command(&commands, cmd);
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (commands);
}
