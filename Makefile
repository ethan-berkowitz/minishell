# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eberkowi <eberkowi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 15:07:12 by eberkowi          #+#    #+#              #
#    Updated: 2024/10/10 15:18:54 by eberkowi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell
CC := cc -g
FLAGS := -Wall -Wextra -Werror
LINKFLAGS := -lreadline

CFILES := main.c \
		error_exit.c \
		parsing/parsing.c \
		parsing/split_input.c \
		parsing/add_elements_to_split_input.c \
		parsing/utilities.c \
		parsing/add_quotes_and_special_elements.c \
		parsing/tokenize.c \
		parsing/free_exit_parsing.c \
		parsing/redirect_tokens.c \
		parsing/add_command_to_struct.c \
		parsing/expand_variables.c \
		parsing/linked_list_utils.c \
		built_ins/exit_command.c \
		environment/environment_utils.c \
		environment/print_environment.c \
		input_and_signals/handle_inputs.c \
		built_ins/echo.c \
		built_ins/export_and_unset.c \
		built_ins/pwd.c \
		built_ins/env.c \
		execution/execute_commandline.c \
		execution/find_path.c \
		execution/get_path.c \

HEADER := includes/minishell.h
LIBFT := libft/libft.a
SRC_PATH := sources/
OBJ_PATH := objects/

CFILES := $(addprefix $(SRC_PATH), $(CFILES))
OBJS := $(CFILES:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH) $(OBJ_PATH)/parsing $(OBJ_PATH)/environment \
	$(OBJ_PATH)/input_and_signals $(OBJ_PATH)/built_ins $(OBJ_PATH)/execution

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LINKFLAGS)

$(LIBFT):
	make -C libft

clean:
	rm -rf $(OBJ_PATH)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all