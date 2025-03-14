/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:27:51 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/10 14:31:07 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define WSPACE 	" \t\n\r\v\f"
# define SEPARATORS " \t\n\r\v\f|><"

# define HERE_DOC_PREFIX "/tmp/.heredoc_"

/*
			ERROR
*/
# define OPERATOR_AT_END "Operator at the end of the command line"
# define OPERTOR_FOLLOW_OPERATOR "Operator followed by another operator"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token `"
# define REDIRECTION_AT_END "syntax error near unexpected token `newline'"
# define WARNING_HERE_DOC_EOF "minishell: warning: \
here-document delimited by end-of-file\n"

typedef struct s_ast_node	t_ast_node;
typedef struct s_file		t_file;
typedef struct s_command	t_command;
typedef struct s_shell		t_shell;
typedef struct s_list		t_list;

typedef enum e_token_type
{
	TYPE_UNKNOW,
	TYPE_COMMAND,
	TYPE_ARG,
	TYPE_PIPE,
	TYPE_REDIRECT_INPUT,
	TYPE_REDIRECT_OUTPUT,
	TYPE_REDIRECT_APPEND,
	TYPE_HERE_DOC,
	TYPE_FILE,
	TYPE_ASSIGNMENT,
	TYPE_VAR_NAME,
	TYPE_VAR_VALUE,
	TYPE_EMPTY
}	t_token_type;

typedef struct s_token
{
	char			*value;
	char			*raw_value;
	t_token_type	type;
}	t_token;

/*
	debug
*/
typedef enum e_node_type	t_node_type;
void		print_command(t_command *command);
const char	*get_type_node_name(t_node_type type);
const char	*get_type_name(t_token_type type);	// a virer
void		print_token(t_token *tokens);
void		print_node_tab(t_ast_node **node_tab);
void		print_ast(t_ast_node *node);

/*
					CHECKER
*/
bool		is_executable(char *command);
bool		is_redirection(t_token token);
bool		is_builtins(char *value);
bool		is_operator(t_token token);
bool		is_command(t_token *tokens, size_t i);

/*
					TOKENIZER
*/
// tokenizer/clean_token.c
char		*del_charset_n(char *str, char *charset, size_t start, size_t end);

// tokenizer/tokenizer_utils.c
char		*skip_whitespaces(char *str);
size_t		ft_strlen_char(char *str, char c);
size_t		get_token_size(char *cmd_line);
void		add_token(t_token **tokens, t_token new_token, bool *error);
t_token		get_last_token(t_token *tokens);
t_token		get_token_bf(t_token *tokens, size_t i);
bool		check_and_do_expand_token(t_shell *shell, t_token *new_token, \
	t_token *ltoken, bool *error);

// tokenizer/tokenizer.c
// c un trim qui fonctionne celui la, compare a certain strtrim
char		*ft_strtrim_llemmel(char const *s1, char const *set);
bool		check_syntax_error(t_token *tokens);
t_token		*tokenize(t_shell *shell, char *cmd_line);

// tokenizer/debug_free.c
void		free_token(t_token *tokens);

/*
					EXPANDER
*/
// expander/skip_simple_quote.c
void		skip_simple_quote(char *value, size_t *i);
// expander/expander_utils.c
char		*get_envvar_value(t_shell shell, char *var_name);
size_t		get_var_size(char *str);
char		*insert_char(char *value, char c, size_t index);
bool		expand_command_path(t_shell shell, t_token *token);
char		*get_split_var_value(t_shell shell, char *var_name, bool in_quotes);
// expander/expander_utils_2.c
char		*get_special_var_value(t_shell shell, char *value, \
	size_t *i, bool in_quotes);
char		*get_var_name(char *value, size_t *name_size);
bool		check_and_do_expand_var(t_shell shell, char **value, \
	size_t *i, bool in_dquote);

//expander/expand_quote.c
bool		expand_quote(char **value);

// expander/expand_var.c
bool		expand_var(t_shell shell, char **value, size_t *i, bool in_quotes);

// expander/expander.c
bool		expand_quote(char **value);
bool		expand_vars(t_shell shell, char **value, bool skip_quote);
bool		expand_token(t_shell shell, char **value);
bool		expander_command(t_shell *shell);

/*
					PARSER
*/
// parser/debug_free.c
void		free_tab(char **tab);
void		free_file(void *addr);
void		free_command(t_command *command);
void		free_tab_node(t_ast_node **node_tab);

// parser/parser_utils.c
bool		ret_false_error(bool *error, t_command *cmd);
size_t		get_tab_size(char **tab);
t_command	*create_command(char *path);
t_file		*create_file(char *path, bool append);
void		add_node(t_ast_node ***node_tab, t_ast_node *new_node, bool *error);

// parser/command_parser_utils.c
bool		open_files(t_shell *shell, t_command *command);
bool		fill_command(t_shell *shell, t_command *cmd, \
						size_t *i, bool *error);
bool		fill_empty_command(t_shell *shell, t_command *cmd, bool *error);

// parser/command_parser.c
bool		add_arg(char ***tab, char *value);
bool		add_cmd_name(t_command *command, \
							t_token *token, bool *error);
bool		add_redirection(t_command *command, t_token *tokens, \
							size_t *i, bool *err);
bool		get_simple_command(t_shell *shell, size_t *i, bool *error);
// parser/parser_pipe.c
bool		get_pipe(t_shell *shell, size_t *i, bool *error);
// parser/parser_heredoc.c
bool		make_here_doc(t_token *tokens, size_t *i, \
							t_ast_node **here_doc, bool *error);
bool		get_heredoc(t_token *tokens, size_t *i, \
						t_ast_node ***node_tab, bool *error);

// parser/parser.c
bool		parser(t_shell *shell);

/*
					HERE_DOC
*/
// here_doc/sup_here_doc.c
bool		sup_here_doc(t_command *command);
// here_doc/exec_here_doc.c
bool		exec_here_doc(t_shell *shell, t_file *file);
// here_doc/here_doc.c
bool		here_doc(t_shell *shell, t_file *file);

/*
					MAKE_AST
*/

// make_ast/make_ast_utils.c
size_t		get_command_count(t_ast_node **node_tab);
bool		get_next_pipe(t_ast_node **node_tab, size_t *pos);

// make_ast/make_ast.c
bool		make_ast(t_shell *shell, t_ast_node **node_tab);

/*
	PARSING MAIN FUNCTION
*/
void		free_var(t_shell *shell, char *command_line_trimmed);
bool		parse_command_line(t_shell *shell, const char *command_line);

#endif