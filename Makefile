##
## EPITECH PROJECT, 2026
## raytracer
## File description:
## Makefile
##

NAME    = raytracer
TESTS = tests_run

CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -O2 -Iinclude
LDFLAGS  = -lconfig++

SRC = main.cpp \
      src/Vector.cpp \
      src/Point3D.cpp \
      src/Camera.cpp \
      src/Rectangle3D.cpp \
      src/primitives/Sphere.cpp \
      src/scene/ConfigParser.cpp \
      src/core/Renderer.cpp

OBJ = $(SRC:.cpp=.o)

TEST_SRC = tests/test_help.cpp \
		   src/Help.cpp \

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete

fclean: clean
	rm -f $(NAME)

re: fclean all

tests_run:
	$(CC) $(CFLAGS) $(TEST_SRC) -lcriterion -o $(TESTS)

.PHONY: all clean fclean re
