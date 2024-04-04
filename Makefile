NAME		= ft_irc
CXX		= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -c -I includes/
#
SRCS		= main.cpp Server.cpp Client.cpp cmd_search.cpp Channel.cpp irssi.cpp utils.cpp \
			old_cmd/0_commands.cpp old_cmd/1_commands.cpp \
			old_cmd/2_commands.cpp old_cmd/3_mode.cpp \
			new_cmd/ping.cpp new_cmd/nick.cpp \
			new_cmd/list.cpp new_cmd/join.cpp \
			new_cmd/privmsg.cpp new_cmd/topic.cpp 

DIR_SRCS	= src/
DIR_OBJS	= .objs/

OBJS		= ${SRCS:%.cpp=${DIR_OBJS}%.o}
DEPFILES	= ${OBJS:.o=.d}

RM 		= rm -f

all:	${NAME}

${NAME} : ${OBJS}
	${CXX} $^ -o $@

${OBJS} : ${DIR_OBJS}%.o: ${DIR_SRCS}%.cpp
	mkdir -p ${@D}
	${CXX} ${CPPFLAGS} $< -o $@
	@${CXX} -MM -MT $@ ${CPPFLAGS} ${DIR_SRCS}$*.cpp > ${DIR_OBJS}$*.d

-include ${DEPFILES}

clean:
	${RM} -r ${DIR_OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re