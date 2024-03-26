NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

SRCS = main.cpp src/server.cpp src/client.cpp src/utils.cpp src/channel.cpp \
		src/auth.cpp src/quit.cpp src/join.cpp src/part.cpp src/mode.cpp
INC = inc/server.hpp inc/client.hpp inc/utils.hpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

%.o: %.cpp $(INC)
	${CC} ${CFLAGS} -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
