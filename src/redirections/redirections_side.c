/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_side.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 22:45:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:54:57 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	se_out(const char *f)
{
	int	fd;

	fd = open(f, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

static int	se_app(const char *f)
{
	int	fd;

	fd = open(f, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

static int	se_in(const char *f)
{
	int	fd;

	fd = open(f, O_RDONLY);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

int	redirections_side_effects_only(t_redir *r, t_shell *shell)
{
	int	code;

	while (r)
	{
		if (r->type == REDIR_OUT && se_out(r->file))
			return (1);
		else if (r->type == REDIR_APPEND && se_app(r->file))
			return (1);
		else if (r->type == REDIR_IN && se_in(r->file))
			return (1);
		else if (r->type == REDIR_HEREDOC)
		{
			code = heredoc_consume_only(r->file, shell);
			if (code != 0)
				return (code);
		}
		r = r->next;
	}
	return (0);
}
