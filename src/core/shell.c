/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:29 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:40:13 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->env = copy_env(env);
	shell->current_input = NULL;
	shell->exit_status = 0;
	shell->tokens = NULL;
	shell->commands = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->tokens)
		free_tokens(&shell->tokens);
	if (shell->commands)
		free_commands(&shell->commands);
	if (shell->env)
		free_array(shell->env);
}

static void	process_input(char *input, t_shell *shell)
{
	add_history(input);
	shell->tokens = tokenize(input);
	if (shell->tokens)
	{
		shell->commands = parse_tokens(shell->tokens);
		if (shell->commands && has_redir_syntax_error(shell->commands))
		{
			shell->exit_status = 2;
			free_commands(&shell->commands);
			free_tokens(&shell->tokens);
		}
		else if (shell->commands)
		{
			shell->exit_status = execute_command(shell->commands, shell);
			free_commands(&shell->commands);
			free_tokens(&shell->tokens);
		}
		else
		{
			free_tokens(&shell->tokens);
		}
	}
	(void)input;
}

static int	read_line_and_prepare(char **out_line)
{
	char	*input;
	char	*line;

	input = readline("minishell$ ");
	if (!input)
	{
		ft_putendl_fd("exit", 1);
		clear_history();
		return (-1);
	}
	line = ms_complete_quotes(input);
	free(input);
	if (!line)
		return (0);
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (0);
	}
	*out_line = line;
	return (1);
}

void	run_shell(t_shell *shell)
{
	char	*line;
	int		st;

	while (1)
	{
		st = read_line_and_prepare(&line);
		if (st < 0)
			break ;
		if (st == 0)
			continue ;
		shell->current_input = line;
		process_input(line, shell);
		if (shell->current_input)
		{
			free(shell->current_input);
			shell->current_input = NULL;
		}
	}
}
