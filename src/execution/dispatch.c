/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:35 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:40:38 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**expand_args(char **args, t_shell *shell)
{
	char	**expanded;
	int		i;

	if (!args)
		return (NULL);
	expanded = malloc(sizeof(char *) * (array_size(args) + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded[i] = expand_variables(args[i], shell);
		if (!expanded[i])
		{
			free_array(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (cmd->next)
		return (execute_pipeline(cmd, shell));
	else
		return (execute_single_command(cmd, shell));
}

int	execute_builtin(char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, shell));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args));
	else if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, shell));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, shell));
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(shell));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, shell));
	return (0);
}

int	execute_external(char **args, t_shell *shell)
{
	char	*path;
	char	*expanded;

	if (!args || !args[0])
		return (0);
	expanded = expand_variables(args[0], shell);
	if (!expanded)
		return (1);
	path = find_command_path(expanded, shell);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": command not found", 2);
		free(expanded);
		return (127);
	}
	execve(path, args, shell->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putendl_fd(": command not found", 2);
	free(expanded);
	free(path);
	return (127);
}
