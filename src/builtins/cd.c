/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:03 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:19:01 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_pwd_vars(t_shell *shell, char *old_pwd, char *new_pwd)
{
	char	*oldpwd_var;
	char	*pwd_var;

	if (old_pwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
		builtin_export((char *[]){"export", oldpwd_var, NULL}, shell);
		free(oldpwd_var);
		free(old_pwd);
	}
	if (new_pwd)
	{
		pwd_var = ft_strjoin("PWD=", new_pwd);
		builtin_export((char *[]){"export", pwd_var, NULL}, shell);
		free(pwd_var);
		free(new_pwd);
	}
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	if (!args[1])
		return (get_env_value("HOME", shell));
	else
		return (args[1]);
}

static int	change_directory(char *path, char *old_pwd)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(old_pwd);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (1);
	}
	path = get_cd_path(args, shell);
	if (!path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (change_directory(path, old_pwd) != 0)
		return (1);
	new_pwd = getcwd(NULL, 0);
	update_pwd_vars(shell, old_pwd, new_pwd);
	return (0);
}
