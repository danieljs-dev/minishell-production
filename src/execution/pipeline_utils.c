/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:25:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:40:52 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_child_pipes(int prev_pipe, int *pipe_fd, t_command *commands)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (commands->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	child_exit_cleanup(t_shell *shell, char **args, int code)
{
	if (args)
		free_array(args);
	if (shell->current_input)
	{
		free(shell->current_input);
		shell->current_input = NULL;
	}
	cleanup_shell(shell);
	exit(code);
}

void	child_dispatch_and_exit(t_command *cmd, t_shell *shell)
{
	char	**expanded_args;
	int		code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	expanded_args = expand_args(cmd->args, shell);
	if (!expanded_args)
		child_exit_cleanup(shell, NULL, 1);
	if (expanded_args[0] && is_builtin(expanded_args[0]))
	{
		code = execute_builtin(expanded_args, shell);
		child_exit_cleanup(shell, expanded_args, code);
	}
	if (expanded_args[0])
	{
		code = execute_external(expanded_args, shell);
		child_exit_cleanup(shell, expanded_args, code);
	}
	child_exit_cleanup(shell, expanded_args, 0);
}
