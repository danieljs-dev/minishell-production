/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_side.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 02:58:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:40 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_child(const char *delimiter)
{
	char	*content;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	content = read_heredoc_lines((char *)delimiter);
	if (!content)
		exit(130);
	free(content);
	exit(0);
}

int	heredoc_consume_only(char *delimiter)
{
	pid_t	pid;
	int		status;

	set_parent_exec_signals();
	pid = fork();
	if (pid == 0)
		heredoc_child(delimiter);
	if (pid < 0)
		return (restore_parent_interactive_signals(), 1);
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	restore_parent_interactive_signals();
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
		return (130);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
