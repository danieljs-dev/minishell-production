/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 01:55:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:53:49 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
