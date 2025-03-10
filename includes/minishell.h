/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:47:01 by benpicar          #+#    #+#             */
/*   Updated: 2025/03/09 18:05:58 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <dirent.h>
# include <errno.h>
# include <sys/types.h>
# include <signal.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "parsing.h"
# include "../libft/includes/libft.h"

# define ERR_CD1 "cd: cannot change directory to: "
# define ERR_CD2 "cd: no such file or directory: "
# define ERR_RSTO "Erreur lors de la redirection de stdout\n"
# define ERR_RSTE "Erreur lors de la redirection de stdout\n"
# define ERR_HERE "minishell: syntax error near unexpected token `"
# define ERR_CD "minishell: cd: error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory\n"

# define SPE_VAR "_=/usr/bin/env\n"

# define WSPACE " \t\n\r\v\f"

# define DEFAULT_PATH \
	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

typedef struct s_token			t_token;
typedef enum e_token_type		t_token_type;

/*Globale variable for signal*/
extern int						g_signal[4];

typedef struct s_env
{
	char			name[255];
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	char		**paths;
	char		*prompt;
	int			argc;
	char		**argv;
	char		**envp;
	int			fd1;
	int			fd2;
	int			pid;
	t_env		*lst_env;
}	t_mini;

typedef struct s_file
{
	char	*name;
	int		fd;
	bool	append;
	bool	expand;
}	t_file;

typedef struct s_command
{
	char	*path;
	char	**args;
	int		fd[2];
	t_list	*input_file;
	t_list	*output_file;
}	t_command;

typedef enum e_node_type
{
	NODE_UNKNOW,
	NODE_COMMAND,
	NODE_PIPE,
}	t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	void				*data;
	struct s_ast_node	*root;
	struct s_ast_node	*parent;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

typedef struct s_var
{
	char	*name;
	char	*value;
}	t_var;

typedef struct s_shell
{
	char			**envp;
	t_ast_node		*ast;
	t_list			*global_var;
	int				last_exit_status;
	size_t			nb_command;
	t_token			*tokens;
	t_ast_node		**node_tab;
	int				*pid;
	int				idx_pid;
	int				**pipe;
	size_t			nb_com;
	bool			edit_terme;
	char			*input;
	char			*prompte;
	bool			save_path;
	char			*pwd;
	pid_t			pid_parent;
	int				status;
	t_command		*c_command;
	char			*trim_command;
	char			*file_name_temp;
}	t_shell;

/* AST */
// ast/create_node.c
t_ast_node	*create_node(void *data, t_ast_node *root, t_node_type type);
// ast/add_node.c
void		add_left(t_ast_node *node_parent, t_ast_node *node_left);
void		add_right(t_ast_node *node_parent, t_ast_node *node_right);
void		add_left_right(t_ast_node *node_parent, \
						t_ast_node *node_left, t_ast_node *node_right);

// ast/free_ast.c
void		free_ast(t_ast_node *ast, void (*del_fct)(void *));

// utils temporaire 
void		free_tab(char **tab);
void		free_command(t_command *command);

// command/command.c
char		*add_default_invalid_path(t_shell shell, char *cmd_name);
char		*search_command(t_shell shell, const char *cmd_name, bool *error);

/* PARSING */
// tokenizer.c
t_token		*tokenize(char *command_line);

int			expand_tokens(t_shell *shell, t_token *tokens);

int			create_node_tab(t_token *tokens, t_ast_node ***node_tab);
int			assemble_ast(t_shell *shell, t_ast_node **node_tab);

bool		sup_here_doc(t_command *command);

/*init*/

t_list		*ft_init_global(char **envp, ssize_t j, \
							t_list *start, t_shell *shell);

/*ft_init_utils*/

char		**ft_init_our_envp(t_list *lst, int i, t_shell *shell);
char		*ft_init_prompt(char *add);
bool		ft_have_four_prim_var(t_list *start);

/*ft_free*/

void		ft_free_array(char ***array_string);
void		ft_free_shell(t_shell *shell);
void		ft_free_pipe(t_shell *shell);
void		ft_error_malloc(t_shell *shell);
void		ft_error(char *str, char *s2);

/*ft_free_child*/
void		free_command2(t_command **command);
void		ft_free_child(t_shell *shell, int exit_status);

/*ft_echo*/

void		ft_echo(t_command *com, t_shell *shell);

/*ft_pwd*/

void		ft_pwd(t_shell *shell);

/*ft_execute*/

bool		ft_execute(t_shell *shell);

/*ft_cd*/

int			ft_cd(t_shell *shell, t_command *com);

/*ft_cd_utils*/

char		*ft_cd_get_home(t_shell *shell);
int			ft_len(char **array_string);
void		ft_sup_here_doc(t_command *com);

/*ft_error_child*/

void		ft_handle_child(int sig, siginfo_t *info, void *context);
void		ft_error_execve(t_shell *shell, t_command *com);

/*ft_execute_wait*/

void		ft_execute_wait(t_shell *shell);
void		ft_edit__var(t_shell *shell, t_command *com);
void		ft_wait_parent(int div);
void		ft_piped_mini(t_command *com, t_shell *shell);

/*ft_env*/

void		ft_env(t_shell *shell);

/*ft_exit*/

void		ft_exit(t_shell *shell, int status);
int			ft_exit_spe(t_shell *shell, t_command *com);

/*ft_export_no_arg*/

void		ft_export_no_arg(t_list *lst, int nb_var, t_shell *shell);
int			ft_is_valid_arg(char *arg, ssize_t j);

/*ft_export_utils*/

bool		ft_export_edit_var(t_list *lst, char *arg, int j);
bool		ft_export_add_new(t_list *lst, char *arg, int j);
bool		ft_export_add_var_null(t_list *lst, char *arg, int j, \
	t_shell *shell);
void		ft_check_return(char *arg, t_shell *shell, ssize_t j, t_list *lst);

/*ft_export*/

int			ft_export(t_shell *shell, t_command *com);
bool		ft_is_exist_already(char *arg, t_list *lst, int j);

/*ft_unset*/

int			ft_unset(t_shell *shell, t_command *com);
void		ft_del_var(void *content);

/*ft_init_shell*/

t_shell		*ft_init_shell(char **envp);
t_shell		*ft_acces_shell(char **envp);

/*ft_execute_utils*/

bool		ft_sigint(t_shell *shell);
bool		ft_sigquit(t_shell *shell);
bool		ft_sigsegv(void);
void		ft_loop_mini(t_shell *shell);
void		ft_last_exit_status(t_shell *shell);

/*ft_pars_exec_last*/

void		ft_pars_exec_last(t_shell *shell, t_command *com);
void		ft_command_last(t_shell *shell, t_command *com);
void		ft_child_last(t_shell *shell, t_command *com);
bool		ft_redir_check_last(t_command *com, t_shell *shell);

/*ft_pars_exec*/

void		ft_pars_exec(t_shell *shell, t_command *com);
void		ft_command(t_shell *shell, t_command *com);
void		ft_child(t_shell *shell, t_command *com);
bool		ft_redir_check(t_command *com, t_shell *shell);
void		ft_close_pipe(t_shell *shell);

/*ft_redir_utils*/

void		ft_input_file(t_command *com, bool *ret);
void		ft_output_file(t_command *com, bool *ret);

/*ft_signal*/

void		ft_ges(int sig, siginfo_t *info, void *context);
void		ft_init_sig(int sig, void (*ft_ges)(int, siginfo_t *, void *));
void		ft_handle_here_doc(int sig, siginfo_t *info, void *context);
void		ft_handle_here_doc_child(int sig, siginfo_t *info, void *context);
void		ft_ges_in_exe(int sig, siginfo_t *info, void *context);

void		ft_loop(t_shell *shell);

#endif