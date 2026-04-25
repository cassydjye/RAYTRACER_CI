##
## EPITECH PROJECT, 2026
## Makefile
## File description:
## Makefile
##

CC = g++
CFLAGS = -Wall -Wextra -I include
NAME = raytracer
TESTS = tests_run

SRC = main.cpp \
	src/Parser.cpp \
	src/Help.cpp \

TEST_SRC = tests/test_help.cpp \
		   src/Help.cpp \

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

tests_run:
	$(CC) $(CFLAGS) $(TEST_SRC) -lcriterion -o $(TESTS)

clean:
	rm -f $(OBJ) $(TESTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re