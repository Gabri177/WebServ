NAME = webserv

SRC_DIR = ./src
INCLUDE_DIR = ./include
OBJ_DIR = ./obj

CPPFILES = main.cpp Config.cpp Server.cpp RequestHandler.cpp HttpRequest.cpp HttpResponse.cpp ParserURL.cpp
CPPFILES := $(addprefix $(SRC_DIR)/, $(CPPFILES))
OFILES = $(addprefix $(OBJ_DIR)/, $(notdir $(CPPFILES:.cpp=.o)))

CPPFLAGS = -g3 -Wall -Wextra -Werror -std=c++98 -I$(INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OFILES)
	c++ $(CPPFLAGS) -o $(NAME) $(OFILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	c++ $(CPPFLAGS) -c -Wshadow $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OFILES)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJ_DIR)

re: fclean all

.PHONY: clean fclean re
