/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 03:00:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:39:57 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	toggle_quote(char c, int *sq, int *dq)
{
	if (c == '\'')
	{
		if (!*dq)
			*sq = !*sq;
		return (1);
	}
	if (c == '"')
	{
		if (!*sq)
			*dq = !*dq;
		return (1);
	}
	return (0);
}

static int	quotes_balanced(const char *s)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (!toggle_quote(s[i], &sq, &dq) && s[i] == '\\' && s[i + 1])
			i++;
		i++;
	}
	return (!sq && !dq);
}

static char	*join_free(char *a, const char *b)
{
	char	*res;

	if (!a)
		return (ft_strdup(b));
	res = ft_strjoin(a, b);
	free(a);
	return (res);
}

char	*ms_complete_quotes(char *first_line)
{
	char	*acc;
	char	*more;

	acc = ft_strdup(first_line);
	free(first_line);
	if (!acc)
		return (NULL);
	while (!quotes_balanced(acc))
	{
		more = readline("> ");
		if (!more)
			break ;
		acc = join_free(acc, "\n");
		acc = join_free(acc, more);
		free(more);
		if (!acc)
			return (NULL);
	}
	return (acc);
}
