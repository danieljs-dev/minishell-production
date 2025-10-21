/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:52 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
    int status;
    int i;

    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], &status, 0);
        i++;
    }
    if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
    else
        shell->exit_status = WEXITSTATUS(status);
    return (shell->exit_status);
}
