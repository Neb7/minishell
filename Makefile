#libft
LIBFT		= libft/
LIB_A		=${LIBFT}/libft.a

OBJS_DIR	= objects/
SRCS_DIR 	= srcs/

#Sources
SRC			= main.c

DIR_AST		= ast/
SRC_AST		= add_node.c \
			  create_node.c \
			  free_ast.c


DIR_COM		= command/
SRC_COM		= command.c

DIR_EXE		= exec/
SRC_EXE		= init.c \
			  ft_execute.c \
			  ft_free.c \
			  ft_signal.c \
			  ft_init_utils.c \
			  ft_exit.c \
			  ft_init_shell.c


DIR_EXE_BUI	= builtins/
SRC_EXE_BUI	= ft_cd.c \
			  ft_cd_utils.c \
			  ft_echo.c \
			  ft_env.c \
			  ft_export_no_arg.c \
			  ft_export_utils.c \
			  ft_export.c \
			  ft_pwd.c \
			  ft_unset.c


DIR_EXE_PAR	= pars_exec/
SRC_EXE_PAR	= ft_execute_utils.c \
			  ft_pars_exec_last.c \
			  ft_pars_exec.c \
			  ft_redir_utils.c \
			  ft_execute_wait.c \
			  ft_free_child.c \
			  ft_error_child.c


DIR_PAR		= parsing/
SRC_PAR		= parse_command_line.c \
			  checker.c \
			  free.c


DIR_PAR_MAS	= make_ast/
SRC_PAR_MAS	= make_ast.c \
			  make_ast_utils.c \

DIR_PAR_PAR	= parser/
SRC_PAR_PAR	= command_parser.c \
			  command_parser_utils.c \
			  command_parser_utils_2.c \
			  parser_pipe.c \
			  parser_utils.c \
			  parser.c

DIR_PAR_HERE_DOC	= here_doc/
SRC_PAR_HERE_DOC	= here_doc.c \
					  exec_here_doc.c \
					  sup_here_doc.c

DIR_PAR_TOK	= tokenizer/
SRC_PAR_TOK	= check_syntax_error.c \
			  tokenizer_utils.c \
			  tokenizer_utils_2.c \
			  clean_token.c \
			  tokenizer.c

DIR_PAR_EXP = expander/
SRC_PAR_EXP = expander.c \
			  expand_var.c \
			  expander_utils.c \
			  expander_utils_2.c \
			  skip_simple_quote.c \
			  expand_quote.c

SRC_PAR		+= $(addprefix ${DIR_PAR_MAS}, ${SRC_PAR_MAS})
SRC_PAR		+= $(addprefix ${DIR_PAR_PAR}, ${SRC_PAR_PAR})
SRC_PAR		+= $(addprefix ${DIR_PAR_TOK}, ${SRC_PAR_TOK})
SRC_PAR		+= $(addprefix ${DIR_PAR_EXP}, ${SRC_PAR_EXP})
SRC_PAR		+= $(addprefix ${DIR_PAR_HERE_DOC}, ${SRC_PAR_HERE_DOC})

SRC_EXE		+= $(addprefix ${DIR_EXE_BUI}, ${SRC_EXE_BUI})
SRC_EXE		+= $(addprefix ${DIR_EXE_PAR}, ${SRC_EXE_PAR})

SRC			+= $(addprefix ${DIR_EXE}, ${SRC_EXE})
SRC			+= $(addprefix ${DIR_PAR}, ${SRC_PAR})
SRC			+= $(addprefix ${DIR_COM}, ${SRC_COM})
SRC			+= $(addprefix ${DIR_AST}, ${SRC_AST})

SRCS		= $(addprefix ${SRCS_DIR}, ${SRC})

#Object
OBJS		= $(addprefix ${OBJS_DIR}, ${SRC:.c=.o})


INCLUDES	= includes
NAME		= minishell
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror -I ${INCLUDES} -g3

#Colors
LIGHT_GRAY	= \033[2m
ORANGE		= \033[1;33m
DARK_GRAY	= \033[0;90m
RED			= \033[0;91m
GREEN		= \033[0;92m
YELLOW		= \033[0;93m
BLUE		= \033[0;94m
MAGENTA		= \033[0;95m
CYAN		= \033[0;96m
WHITE		= \033[0;97m
RESET		= \033[0m

#Forme
BOLD		= \033[1m
ITALIC		= \033[3m
UNDERLINE	= \033[4m
CROSS		= \033[9m
FLASH		= \033[5m
NEGATIF		= \033[7m

all:			${NAME}

${OBJS_DIR}%.o: ${SRCS_DIR}%.c | ${OBJS_DIR}
				@cc ${CFLAGS} -c $< -o $@

${NAME}:		${LIB_A} ${OBJS}
				@${CC} ${CFLAGS} ${OBJS} -L${LIBFT} -lft -lreadline -o $@ 
				@echo "${YELLOW}'$@' is compiled ! ✅${RESET}"

${OBJS_DIR}:
				@mkdir -p ${OBJS_DIR}
				@mkdir -p ${OBJS_DIR}${DIR_EXE}
				@mkdir -p ${OBJS_DIR}${DIR_EXE}${DIR_EXE_BUI}
				@mkdir -p ${OBJS_DIR}${DIR_EXE}${DIR_EXE_PAR}
				@mkdir -p ${OBJS_DIR}${DIR_COM}
				@mkdir -p ${OBJS_DIR}${DIR_AST}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}${DIR_PAR_MAS}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}${DIR_PAR_PAR}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}${DIR_PAR_TOK}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}${DIR_PAR_EXP}
				@mkdir -p ${OBJS_DIR}${DIR_PAR}${DIR_PAR_HERE_DOC}

${LIBFT}:		
				@git clone https://github.com/Neb7/libft.git $@

${LIB_A}:		${LIBFT}
				@echo "${BLUE}'libft.a' is compiling... 🔥${RESET}"
				@make -sC ${LIBFT}

clean:
				@${RM} ${OBJS}
				@${RM} -r ${OBJS_DIR}
				@make clean -sC ${LIBFT}
				@echo "${RED}'${NAME}' objects are deleted ! 👍${RESET}"

fclean:			clean
				@${RM} ${NAME} ${LIB_A}
				@echo "${RED}'${NAME}' and 'libft.a' are deleted ! 👍${RESET}"

re:				fclean all

.PHONY:			all clean fclean re
