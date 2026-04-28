##
## EPITECH PROJECT, 2026
## raytracer
## File description:
## Makefile
##

NAME    = raytracer

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

.PHONY: all clean fclean re
