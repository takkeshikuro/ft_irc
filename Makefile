NAME		= ircserv
CXX		= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -c -I includes/
#
SRCS		= main.cpp Server.cpp Server_utils.cpp Client.cpp \
			cmd_search.cpp Channel.cpp irssi.cpp utils.cpp \
			commands/netcat_exclu.cpp \
			commands/ping.cpp commands/nick.cpp \
			commands/list.cpp commands/join.cpp \
			commands/privmsg.cpp commands/topic.cpp \
			commands/user.cpp commands/part.cpp commands/kick.cpp \
			commands/mode/mode.cpp commands/mode/mode_l.cpp \
			commands/mode/mode_utils.cpp commands/mode/mode_o.cpp \
			commands/mode/mode_i.cpp commands/mode/mode_t.cpp \
			commands/pass.cpp commands/invite.cpp commands/mode/mode_k.cpp \
			commands/quit.cpp commands/bot.cpp commands/login.cpp

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