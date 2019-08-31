# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amansour <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/29 13:33:28 by amansour          #+#    #+#              #
#    Updated: 2019/08/21 15:12:02 by amansour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
# SOURCES       															   #
################################################################################

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_H = $(LIBFT_DIR)/inc

################################################################################
#################################### RULES #####################################
################################################################################

all : $(LIBFT)
	@make -C ft_nm
	@printf "\033[1;33m%s\033[0m\n" "FT_NM"

$(LIBFT) :
	@make -C $(LIBFT_DIR)
	@printf "%-45s\033[1;32m%s\033[0m\n" "Make $@" "OK"

clean :
	@make -C ft_nm clean


fclean : clean
	@make -C ft_nm fclean


fclean_lib :
	@make fclean -C $(LIBFT_DIR)

fclean_all : fclean_lib fclean

re : fclean all

re_with_lib: fclean_all all

.PHONY: all clean fclean re fclean_lib fclean_all re_with_lib