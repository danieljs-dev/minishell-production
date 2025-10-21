/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:19 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void hd_sigint(int sig)
{
    (void)sig;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    write(1, "^C\n", 3);
    exit(130);
}

static void rl_swap_to_tty(t_rl_io *io)
{
    io->old_in = rl_instream;
    io->old_out = rl_outstream;
    io->in = fopen("/dev/tty", "r");
    io->out = fopen("/dev/tty", "w");
    if (io->in)
        rl_instream = io->in;
    if (io->out)
        rl_outstream = io->out;
}

static char *append_line_to_input(char *input, char *line)
{
    char *temp;
    char *result;

    if (input)
    {
        temp = ft_strjoin(input, "\n");
        free(input);
        result = ft_strjoin(temp, line);
        free(temp);
        free(line);
        return (result);
    }
    else
        return (line);
}

static char *hd_loop(const char *delim)
{
    char *line;
    char *input;

    input = NULL;
    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break ;
        }
        input = append_line_to_input(input, line);
    }
    if (!input)
        return (ft_strdup(""));
    return (input);
}

char *read_heredoc_lines(char *delimiter)
{
    t_rl_io io;
    char    *input;
    struct sigaction sa, sa_old;

    rl_swap_to_tty(&io);
    sa.sa_handler = hd_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, &sa_old);
    input = hd_loop(delimiter);
    sigaction(SIGINT, &sa_old, NULL);
    if (io.in)
        fclose(io.in);
    if (io.out)
        fclose(io.out);
    rl_instream = io.old_in;
    rl_outstream = io.old_out;
    return (input);
}
