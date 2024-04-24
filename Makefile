NAME = ircserv
NAME_B = bot

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

BONUS_SRCS = bonus/bot.cpp bonus/CBot.cpp bonus/utils.cpp \
		src/server.cpp src/client.cpp src/utils.cpp src/channel.cpp \
		src/auth.cpp src/quit.cpp src/join.cpp src/part.cpp src/mode.cpp \
		src/invite.cpp src/topic.cpp src/kick.cpp src/privmsg.cpp



SRCS = main.cpp src/server.cpp src/client.cpp src/utils.cpp src/channel.cpp \
		src/auth.cpp src/quit.cpp src/join.cpp src/part.cpp src/mode.cpp \
		src/invite.cpp src/topic.cpp src/kick.cpp src/privmsg.cpp
INC = inc/server.hpp inc/client.hpp inc/utils.hpp inc/replies.hpp inc/utils.hpp inc/topic.hpp bonus/CBot.hpp
OBJS = $(SRCS:.cpp=.o)

OBJS_B = $(BONUS_SRCS:.cpp=.o)

all: $(NAME)

%.o: %.cpp $(INC)
	${CC} ${CFLAGS} -c $< -o $@

bonus: $(NAME_B)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)  -lcurl

$(NAME_B) : $(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) -o $(NAME_B) -lcurl


clean:
	$(RM) $(OBJS) $(OBJS_B)

fclean: clean
	$(RM) $(NAME) $(NAME_B)

re: fclean all 

.PHONY: all clean fclean re bonus
