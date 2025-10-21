/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 03:12:00 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/18 03:12:00 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_builtin_case(char **expanded_args, t_command *cmd, t_shell *shell)
{
    int code;

    if (!expanded_args[0] || !is_builtin(expanded_args[0]))
        return (-1);
    code = run_builtin_in_parent(expanded_args, cmd, shell);
    free_array(expanded_args);
    return (code);
}

static void handle_child_process(t_command *cmd, char **expanded_args, t_shell *shell)
{
    int exit_code;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (setup_redirections(cmd->redirs) == 0)
    {
        if (expanded_args[0] && is_builtin(expanded_args[0]))
            exit_code = execute_builtin(expanded_args, shell);
        else
            exit_code = execute_external(expanded_args, shell);
        free_array(expanded_args);
        exit(exit_code);
    }
    free_array(expanded_args);
    exit(1);
}

static int handle_fork_and_wait(t_command *cmd, char **expanded_args, t_shell *shell)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == 0)
    {
        handle_child_process(cmd, expanded_args, shell);
        return (1);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
        else
            shell->exit_status = WEXITSTATUS(status);
        return (shell->exit_status);
    }
    else
        return (1);
}

int exec_single_builtin_or_fork(t_command *cmd, t_shell *shell, char **expanded_args)
{
    int builtin_result;
    int result;

    builtin_result = handle_builtin_case(expanded_args, cmd, shell);
    if (builtin_result != -1)
        return (builtin_result);
    set_parent_exec_signals();
    result = handle_fork_and_wait(cmd, expanded_args, shell);
    restore_parent_interactive_signals();
    free_array(expanded_args);
    cleanup_heredocs(cmd);
    return (result);
}

int prepare_single_heredoc(t_command *cmd, t_shell *shell)
{
    int hd_rc;

    hd_rc = prepare_heredocs(cmd, shell);
    if (hd_rc != 0)
        return (shell->exit_status = hd_rc);
    return (0);
}
