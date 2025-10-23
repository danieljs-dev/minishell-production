/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prepare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 01:55:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 21:01:30 by dajesus-         ###   ########.fr       */
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

static int	hd_finalize_parent(t_redir *r, char *tmp, int status)
{
	int	code;

	restore_parent_interactive_signals();
	code = hd_status_to_code(status, tmp);
	if (code != 0)
		return (code);
	free(r->file);
	r->file = tmp;
	r->type = REDIR_IN;
	return (0);
}

static void	hd_child_exit_cleanup(t_shell *shell, int code)
{
	if (shell->current_input)
	{
		free(shell->current_input);
		shell->current_input = NULL;
	}
	cleanup_shell(shell);
	exit(code);
}

static int	run_one_heredoc(t_redir *r, t_shell *shell)
{
	char	*tmp;
	pid_t	pid;
	int		status;
	int		rc;

	tmp = ms_tmpname();
	if (!tmp)
		return (1);
	set_parent_exec_signals();
	pid = fork();
	if (pid == 0)
	{
		hd_setup_child_signals();
		rc = hd_child_write(tmp, r->file, (r->quote_type == '\0'), shell);
		free(tmp);
		hd_child_exit_cleanup(shell, rc);
	}
	if (pid < 0)
	{
		restore_parent_interactive_signals();
		free(tmp);
		return (1);
	}
	hd_wait_child(pid, &status);
	return (hd_finalize_parent(r, tmp, status));
}

static int	prepare_cmd_heredocs(t_redir *r, t_shell *shell)
{
	int	rc;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			rc = run_one_heredoc(r, shell);
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

	c = cmds;
	while (c)
	{
		code = prepare_cmd_heredocs(c->redirs, shell);
		if (code != 0)
			return (code);
		c = c->next;
	}
	return (0);
}
