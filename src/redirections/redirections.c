/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:05:23 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/15 22:18:45 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_redir_in(char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(file, 2);
        ft_putendl_fd(": No such file or directory", 2);
        return (1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}

static int handle_redir_out(char *file)
{
    int fd;

    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(file, 2);
        ft_putendl_fd(": Permission denied", 2);
        return (1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

static int handle_redir_append(char *file)
{
    int fd;

    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(file, 2);
        ft_putendl_fd(": Permission denied", 2);
        return (1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

static int process_redir_stdin_first(t_redir *redir)
{
    if (redir->type == REDIR_HEREDOC)
        return (0);
    if (redir->type == REDIR_IN)
        return (handle_redir_in(redir->file));
    return (0);
}

int setup_redirections(t_redir *redirs)
{
    t_redir *r;

    r = redirs;
    while (r)
    {
        if (process_redir_stdin_first(r) != 0)
            return (1);
        r = r->next;
    }
    r = redirs;
    while (r)
    {
        if (r->type == REDIR_OUT)
        {
            if (handle_redir_out(r->file) != 0)
                return (1);
        }
        else if (r->type == REDIR_APPEND)
        {
            if (handle_redir_append(r->file) != 0)
                return (1);
        }
        r = r->next;
    }
    return (0);
}
