/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:39 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_signal = 0;

int main(int argc, char **argv, char **env)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, env);
    setup_signals();
    run_shell(&shell);
    cleanup_shell(&shell);
    return (shell.exit_status);
}