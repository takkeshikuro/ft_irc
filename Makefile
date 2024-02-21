NAME	=	ft_irc

CC		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98 

SRCS_DIR	=	./srcs/
SRCS	=	$(wildcard $(SRCS_DIR)*.cpp)

OBJS	=	$(SRCS:.cpp=.o)

INC		=	./inc/
#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

all:	$(NAME)

$(NAME): $(OBJS) $(wildcard $(INC)*.hpp) ./Makefile
	@printf "$(CURSIVE)$(GRAY) 	- Compiling $(NAME)... $(RESET)\n"
	@ $(CC) $(FLAGS)  $(OBJS) -o $(NAME)
	@printf "$(GREEN)    - Executable ready.\n$(RESET)"

clean:
	@rm -f $(OBJS)
	@printf "$(YELLOW)    - .o removed.$(RESET)\n"

fclean:	clean
	@rm -f $(NAME)
	@printf "$(YELLOW)    - Executable removed.$(RESET)\n"

re:	fclean all

.PHONY: all clean fclean re