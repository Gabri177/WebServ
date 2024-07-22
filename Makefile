NAME = webserv

SRC_DIR = ./src
INCLUDE_DIR = ./include


CPPFILES = main.cpp Config.cpp Server.cpp RequestHandler.cpp HttpRequest.cpp HttpResponse.cpp ParserURL.cpp
CPPFILES := $(addprefix $(SRC_DIR)/, $(CPPFILES))
OFILES = $(CPPFILES:.cpp=.o)

CPPFLAGS = -g3 -Wall -Wextra -Werror -std=c++98 -I$(INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OFILES)
	c++ $(CPPFLAGS) -o $(NAME) $(OFILES)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	c++ $(CPPFLAGS) -c -Wshadow $< -o $@

clean:
	$(RM) $(OFILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re
