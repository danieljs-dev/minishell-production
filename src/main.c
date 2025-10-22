/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:39 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:57:06 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (127);
		}
		close(fd);
	}
	init_shell(&shell, env);
	setup_signals();
	run_shell(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
