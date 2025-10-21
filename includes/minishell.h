/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dajesus- <dajesus-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:58:52 by dajesus-          #+#    #+#             */
/*   Updated: 2025/10/21 18:38:38 by dajesus-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define MAX_PATH 4096
# define MAX_ARGS 1000

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	char				quote_type;
	struct s_token		*next;
}						t_token;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*file;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				**env;
	int					exit_status;
	t_token				*tokens;
	t_command			*commands;
}						t_shell;

typedef struct s_word_info
{
	char				*content;
	char				quote_type;
}						t_word_info;

typedef struct s_rl_io
{
	FILE				*old_in;
	FILE				*old_out;
	FILE				*in;
	FILE				*out;
}						t_rl_io;

extern int				g_signal;

int						main(int argc, char **argv, char **env);
void					init_shell(t_shell *shell, char **env);
void					cleanup_shell(t_shell *shell);
void					run_shell(t_shell *shell);
char					*ms_complete_quotes(char *first_line);

// lexer
t_token					*create_token(t_token_type type, char *value);
t_token					*create_quoted_token(t_token_type type, char *value,
							char quote_type);
void					add_token(t_token **tokens, t_token *new_token);
void					free_tokens(t_token **tokens);
t_token					*tokenize(char *input);
int						is_quote(char c);
int						is_redir(char c);
int						is_pipe(char c);
int						is_space(char c);
int						should_continue_extraction(char *input, int i);
t_word_info				extract_word_with_quotes(char *input, int *i);

// parser
t_command				*parse_tokens(t_token *tokens);
int						has_redir_syntax_error(t_command *cmds);
int						count_args(t_token *tokens);
char					**build_args(t_token *tokens, int count);
t_redir					*build_redirs(t_token *tokens);
int						is_redir_token(t_token_type type);
void					free_commands(t_command **commands);
t_redir					*create_redir(t_redir_type type, char *file);
void					add_redir(t_redir **redirs, t_redir *new_redir);
void					free_redirs(t_redir **redirs);
t_command				*create_command(char **args, t_redir *redirs);
void					add_command(t_command **commands, t_command *new_cmd);

// expansion
char					*expand_variables(char *input, t_shell *shell);
char					*expand_variables_with_quotes(char *input,
							char quote_type, t_shell *shell);
char					*get_env_value(char *name, t_shell *shell);
char					*expand_exit_status(char *input, t_shell *shell);
char					*expand_exit_var(char *prefix, char *end,
							t_shell *shell);
char					*expand_env_var(char *prefix, char *start, char *end,
							t_shell *shell);

// builtins
int						builtin_echo(char **args);
int						builtin_cd(char **args, t_shell *shell);
int						builtin_pwd(char **args);
int						builtin_export(char **args, t_shell *shell);
int						builtin_unset(char **args, t_shell *shell);
int						builtin_env(t_shell *shell);
int						builtin_exit(char **args, t_shell *shell);
int						is_builtin(char *cmd);
int						is_valid_identifier(char *name);

// execution
int						execute_command(t_command *cmd, t_shell *shell);
int						execute_pipeline(t_command *commands, t_shell *shell);
int						execute_single_command(t_command *cmd, t_shell *shell);
int						execute_builtin(char **args, t_shell *shell);
int						execute_external(char **args, t_shell *shell);
char					*find_command_path(char *cmd, t_shell *shell);
char					**expand_args(char **args, t_shell *shell);
int						wait_for_children(pid_t *pids, int cmd_count,
							t_shell *shell);
int						exec_single_builtin_or_fork(t_command *cmd,
							t_shell *shell, char **expanded_args);
int						prepare_single_heredoc(t_command *cmd, t_shell *shell);
/* pipeline helpers */
void					setup_child_pipes(int prev_pipe, int *pipe_fd,
							t_command *commands);
void					child_dispatch_and_exit(t_command *cmd, t_shell *shell);
/* run builtin in parent (handles redirs) */
int						run_builtin_in_parent(char **expanded_args,
							t_command *cmd, t_shell *shell);

// redirections
int						setup_redirections(t_redir *redirs);
char					*read_heredoc_lines(char *delimiter);
int						redirections_side_effects_only(t_redir *redirs);
int						prepare_heredocs(t_command *cmds, t_shell *shell);
void					cleanup_heredocs(t_command *cmds);
int						heredoc_consume_only(char *delimiter);
// int     heredoc_consume_only(char *delimiter);

// signals
void					setup_signals(void);
void					signal_handler(int sig);
void					signal_handler_child(int sig);
void					set_parent_exec_signals(void);
void					restore_parent_interactive_signals(void);

// utils
char					**copy_env(char **env);
void					free_array(char **array);
int						array_size(char **array);
char					*join_path(char *path1, char *path2);

#endif
