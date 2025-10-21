/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prepare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 01:55:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:32 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ms_tmpname(void)
{
	static int	n;
	char		*num;
	char		*name;

	num = ft_itoa(n++);
	if (!num)
		return (NULL);
	name = ft_strjoin("/tmp/ms_hd_", num);
	free(num);
	return (name);
}

static int	hd_child_write(const char *path, const char *delim)
{
	int		fd;
	char	*content;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	content = read_heredoc_lines((char *)delim);
	if (!content)
		return (130);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (free(content), 1);
	if (*content)
		write(fd, content, ft_strlen(content));
	close(fd);
	free(content);
	return (0);
}

static int	run_one_heredoc(t_redir *r)
{
	char	*tmp;
	pid_t	pid;
	int		status;

	tmp = ms_tmpname();
	if (!tmp)
		return (1);
	set_parent_exec_signals();
	pid = fork();
	if (pid == 0)
		exit(hd_child_write(tmp, r->file));
	if (pid < 0)
		return (restore_parent_interactive_signals(), free(tmp), 1);
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	restore_parent_interactive_signals();
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
		return (unlink(tmp), free(tmp), 130);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (unlink(tmp), free(tmp), 1);
	free(r->file);
	r->file = tmp;
	r->type = REDIR_IN;
	return (0);
}

static int	prepare_cmd_heredocs(t_redir *r)
{
	int	rc;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			rc = run_one_heredoc(r);
			if (rc != 0)
				return (rc);
		}
		r = r->next;
	}
	return (0);
}

int	prepare_heredocs(t_command *cmds, t_shell *shell)
{
	t_command	*c;
	int			code;

	(void)shell;
	c = cmds;
	while (c)
	{
		code = prepare_cmd_heredocs(c->redirs);
		if (code != 0)
			return (code);
		c = c->next;
	}
	return (0);
}
