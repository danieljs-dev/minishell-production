/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 00:12:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:40:32 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	save_stdio(int fds[2])
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
	{
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
		return (1);
	}
	return (0);
}

static void	restore_stdio(int fds[2])
{
	if (fds[0] != -1)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if (fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
}

int	run_builtin_in_parent(char **expanded_args, t_command *cmd, t_shell *shell)
{
	int	fds[2];

	if (!expanded_args[0] || !is_builtin(expanded_args[0]))
		return (-1);
	if (!cmd->redirs)
		return (shell->exit_status = execute_builtin(expanded_args, shell));
	if (save_stdio(fds) != 0)
		return (1);
	if (setup_redirections(cmd->redirs) == 0)
		shell->exit_status = execute_builtin(expanded_args, shell);
	restore_stdio(fds);
	return (shell->exit_status);
}
