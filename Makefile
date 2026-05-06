##
## EPITECH PROJECT, 2026
## raytracer
## File description:
## Makefile
##

NAME    = raytracer
TESTS = tests_run

CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -O2 -Iinclude -fopenmp
LDFLAGS  = -lconfig++ -lgomp

SRC = main.cpp \
      src/Help.cpp \
      src/Parser.cpp \
      src/math/Point3D.cpp \
      src/math/Vector3D.cpp \
      src/core/Renderer.cpp \
      src/primitives/Sphere.cpp \
      src/primitives/Plane.cpp \
      src/primitives/Cylinder.cpp \
      src/primitives/Triangles.cpp \
      src/primitives/Transform.cpp \
      src/primitives/BVHNode.cpp \
     src/primitives/Mesh.cpp \
      src/scene/ObjLoader.cpp \
      src/scene/ConfigParser.cpp \
      src/raytracer/Camera.cpp \
      src/raytracer/Rectangle3D.cpp


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
	ls

.PHONY: all clean fclean re
