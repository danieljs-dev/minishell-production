/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 01:55:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 21:01:30 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	hd_setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

int	hd_wait_child(pid_t pid, int *status)
{
	while (waitpid(pid, status, 0) == -1 && errno == EINTR)
		continue ;
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	return (0);
}

int	hd_status_to_code(int status, char *tmp)
{
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
		return (unlink(tmp), free(tmp), 130);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (unlink(tmp), free(tmp), 1);
	return (0);
}

void	cleanup_heredocs(t_command *cmds)
{
	t_command	*c;
	t_redir		*r;

	c = cmds;
	while (c)
	{
		r = c->redirs;
		while (r)
		{
			if (r->type == REDIR_IN && r->file && ft_strncmp(r->file,
					"/tmp/ms_hd_", 10) == 0)
			{
				unlink(r->file);
			}
			r = r->next;
		}
		c = c->next;
	}
}
