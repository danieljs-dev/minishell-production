/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:16 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int validate_exit_arg(char *arg)
{
    int i;

    i = 0;
    if (arg[i] == '+' || arg[i] == '-')
        i++;
    if (!arg[i])
        return (0);
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            return (0);
        i++;
    }
    return (1);
}

static int normalize_exit_code(int exit_code)
{
    if (exit_code < 0)
        exit_code = 256 + exit_code;
    else if (exit_code > 255)
        exit_code = exit_code % 256;
    return (exit_code);
}

static void exit_numeric_error(char **args, t_shell *shell)
{
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(args[1], 2);
    ft_putendl_fd(": numeric argument required", 2);
    if (args)
        free_array(args);
    cleanup_shell(shell);
    clear_history();
    exit(2);
}

static int compute_exit_code(char **args, t_shell *shell)
{
    if (!args[1])
        return (shell->exit_status);
    if (!validate_exit_arg(args[1]))
        exit_numeric_error(args, shell);
    return (normalize_exit_code(ft_atoi(args[1])));
}

int builtin_exit(char **args, t_shell *shell)
{
    int exit_code;

    if (args[1] && args[2])
        return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
    ft_putendl_fd("exit", 1);
    exit_code = compute_exit_code(args, shell);
    if (args)
        free_array(args);
    cleanup_shell(shell);
    clear_history();
    exit(exit_code);
}
