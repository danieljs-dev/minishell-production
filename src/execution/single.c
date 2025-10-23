/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:48 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:41:47 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	redir_syntax_error(t_redir *r)
{
	while (r)
	{
		if (!r->file || r->file[0] == '\0')
			return (1);
		r = r->next;
	}
	return (0);
}

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	char	**expanded_args;

	if (redir_syntax_error(cmd->redirs))
		return ((shell->exit_status = 2), 2);
	if (!cmd->args || !cmd->args[0])
	{
		if (redirections_side_effects_only(cmd->redirs, shell) == 0)
			return ((shell->exit_status = 0), 0);
		return (1);
	}
	if (prepare_single_heredoc(cmd, shell))
		return (shell->exit_status);
	expanded_args = expand_args(cmd->args, shell);
	if (!expanded_args)
		return (1);
	return (exec_single_builtin_or_fork(cmd, shell, expanded_args));
}
