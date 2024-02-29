NAME		= ft_irc
CXX		= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -c -I includes/

SRCS		= main.cpp Server.cpp User.cpp
DIR_SRCS	= src/
DIR_OBJS	= .objs/

OBJS		= ${SRCS:%.cpp=${DIR_OBJS}%.o}
DEP		= ${OBJS:%.o=%.d}
RM 		= rm -f

all:	${NAME}

${NAME} : ${OBJS}
	${CXX} $^ -o $@

${OBJS} : ${DIR_OBJS}%.o: ${DIR_SRCS}%.cpp
	mkdir -p ${@D}
	${CXX} ${CPPFLAGS} $< -o $@
-include ${DEP}

clean:
	${RM} -r ${DIR_OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
