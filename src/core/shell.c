/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:03:29 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void init_shell(t_shell *shell, char **env)
{
    shell->env = copy_env(env);
    shell->exit_status = 0;
    shell->tokens = NULL;
    shell->commands = NULL;
}

void cleanup_shell(t_shell *shell)
{
    if (shell->tokens)
        free_tokens(&shell->tokens);
    if (shell->commands)
        free_commands(&shell->commands);
    if (shell->env)
        free_array(shell->env);
}

static void process_input(char *input, t_shell *shell)
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
            return ;
        }
        if (shell->commands)
        {
            shell->exit_status = execute_command(shell->commands, shell);
            free_commands(&shell->commands);
        }
        free_tokens(&shell->tokens);
    }
}

void run_shell(t_shell *shell)
{
    char *input;
    char *line;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            ft_putendl_fd("exit", 1);
            clear_history();
            break;
        }
        line = ms_complete_quotes(input);
        if (ft_strlen(line) > 0)
            process_input(line, shell);
        free(line);
    }
}
