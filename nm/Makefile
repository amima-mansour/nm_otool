# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amansour <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/29 13:33:28 by amansour          #+#    #+#              #
#    Updated: 2019/09/02 12:08:17 by amansour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
# SOURCES       															   #
################################################################################

SRCS		= $(addprefix $(PATH_SRC)/, \
							main.c\
							handle.c\
							fat32.c\
							fat64.c\
							get_arch_name.c\
							archive.c\
							output.c\
							print.c\
							flags.c\
							sections.c\
							swap.c\
							symbol_type.c\
							sort.c\
							alpha_sort.c\
							num_sort.c\
							lst_tools.c\
							errors.c)

################################################################################
# BASIC VARIABLES															   #
################################################################################

PATH_OBJ	= obj
PATH_SRC	= src
PATH_INC	= inc

NAME		= ft_nm
CFLAGS		= -Wall -Wextra -Werror
OBJECTS 	= $(SRCS:$(PATH_SRC)/%.c=$(PATH_OBJ)/%.o)
LIBRARY     = ../libft/libft.a
################################################################################
# RULES																		   #
################################################################################

all: $(NAME)

$(NAME): $(LIBRARY) $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@
	@printf "%-45s\033[1;32m%s\033[0m\n" "Make $@" "OK"

$(PATH_OBJ)/%.o: $(addprefix $(PATH_SRC)/,%.c)
	@mkdir -p $(PATH_OBJ)
	@gcc -c -o $@ $(CFLAGS) $^ -I $(PATH_INC)/
	@printf "%-45s\033[1;32m%s\033[0m\n" "Make $@" "OK"
$(LIBRARY):
	@make -C ../libft/
	@printf "%-45s\033[1;32m%s\033[0m\n" "Make $@" "OK"

# - - - - - - - - — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —

clean:
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(PATH_OBJ)
	@make clean -C ../libft/

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C ../libft/

re: fclean all

re: fclean $(NAME)

# — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —

.PHONY: all clean fclean re nc nh na

# — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —
