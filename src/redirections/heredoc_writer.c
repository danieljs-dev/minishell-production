/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_writer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:25:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 20:44:37 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	hd_append(char **acc, const char *suffix)
{
	char	*tmp;

	tmp = *acc;
	*acc = ft_strjoin(*acc, suffix);
	free(tmp);
	return (*acc == NULL);
}

static char	*hd_accumulate(char **lines, t_shell *shell)
{
	char	*acc;
	int		i;
	char	*expanded;

	acc = ft_strdup("");
	if (!acc)
	{
		free_array(lines);
		return (NULL);
	}
	i = 0;
	while (lines[i])
	{
		expanded = expand_variables(lines[i], shell);
		if (!expanded)
			return (free(acc), free_array(lines), NULL);
		if (hd_append(&acc, expanded))
			return (free(expanded), free_array(lines), NULL);
		free(expanded);
		if (lines[i + 1] && hd_append(&acc, "\n"))
			return (free_array(lines), NULL);
		i++;
	}
	free_array(lines);
	return (acc);
}

static char	*expand_heredoc_content(char *content, t_shell *shell)
{
	char	**lines;

	if (!content)
		return (NULL);
	lines = ft_split(content, '\n');
	if (!lines)
		return (NULL);
	return (hd_accumulate(lines, shell));
}

static int	hd_ensure_nl(char **s)
{
	char	*tmp;

	if (!*s || !**s)
		return (0);
	if ((*s)[ft_strlen(*s) - 1] == '\n')
		return (0);
	tmp = ft_strjoin(*s, "\n");
	free(*s);
	*s = tmp;
	return (*s == NULL);
}

int	hd_child_write(const char *path, const char *delim, int do_expand,
		t_shell *shell)
{
	int		fd;
	char	*content;
	char	*to_write;

	content = read_heredoc_lines((char *)delim);
	if (!content)
		return (130);
	if (do_expand)
	{
		to_write = expand_heredoc_content(content, shell);
		free(content);
	}
	else
		to_write = content;
	if (!to_write || hd_ensure_nl(&to_write))
		return (1);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (free(to_write), 1);
	write(fd, to_write, ft_strlen(to_write));
	close(fd);
	free(to_write);
	return (0);
}
