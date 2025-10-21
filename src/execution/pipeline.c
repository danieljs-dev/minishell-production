/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:02:32 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:40:58 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execute_child_process(t_command *cmd, t_shell *shell, int prev_pipe,
		int *pipe_fd)
{
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(prev_pipe, pipe_fd, cmd);
	if (setup_redirections(cmd->redirs) == 0)
		child_dispatch_and_exit(cmd, shell);
	exit(1);
}

static void	parent_after_fork(t_command *cmd, int *prev_pipe, int *pipe_fd,
		int ok)
{
	if (ok)
	{
		if (*prev_pipe != -1)
			close(*prev_pipe);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			*prev_pipe = pipe_fd[0];
		}
	}
	else
	{
		if (cmd->next)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		if (*prev_pipe != -1)
			close(*prev_pipe);
	}
}

static pid_t	run_one_command(t_command **cmd, t_shell *shell, int *prev_pipe)
{
	int		pipe_fd[2];
	pid_t	pid;

	if ((*cmd)->next && pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
		execute_child_process(*cmd, shell, *prev_pipe, pipe_fd);
	else
		parent_after_fork(*cmd, prev_pipe, pipe_fd, (pid > 0));
	*cmd = (*cmd)->next;
	return (pid);
}

static int	spawn_all(t_command *commands, t_shell *shell, pid_t *pids,
		int state[2])
{
	pid_t	pid;

	while (commands)
	{
		pid = run_one_command(&commands, shell, &state[0]);
		if (pid < 0)
			return (-1);
		pids[(state[1])++] = pid;
	}
	return (0);
}

int	execute_pipeline(t_command *commands, t_shell *shell)
{
	pid_t	pids[MAX_ARGS];
	int		state[2];
	int		rc;
	pid_t	pid;

	if (has_redir_syntax_error(commands))
		return (shell->exit_status = 2);
	pid = prepare_heredocs(commands, shell);
	if (pid != 0)
		return (shell->exit_status = pid);
	state[0] = -1;
	set_parent_exec_signals();
	state[1] = 0;
	rc = spawn_all(commands, shell, pids, state);
	if (rc < 0)
		return (restore_parent_interactive_signals(), 1);
	if (state[0] != -1)
		close(state[0]);
	pid = wait_for_children(pids, state[1], shell);
	restore_parent_interactive_signals();
	cleanup_heredocs(commands);
	return (pid);
}
