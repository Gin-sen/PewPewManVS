NAME	=	BombermanVS
CC	=	gcc -g -Wall -Wextra -Werror -std=c17 $(shell pkg-config --cflags --libs sdl2) -lSDL2_image -lSDL2_ttf -lSDL2_mixer
RM	=	rm -f
SRC	=	./src/game.c \
		./src/menu.c
OBJ	=	$(SRC:%.c=%.o)
LFLAGS	=	-L./lib -lmy_lib_$(shell uname -m)-$(shell uname -s)

all:					nolib

nolib:					$(OBJ)
					$(CC) ./main.c $(OBJ) -Iinclude -o $(NAME) 

withmylibstat:					$(OBJ)
						make -f ./MakeMyLib fclean my_lib_static
					$(CC) ./main.c $(OBJ) $(LFLAGS) -Iinclude -o $(NAME) $(shell pkg-config --cflags --libs sdl2) -lSDL2_image
clean:
					$(RM) $(OBJ)

fclean:					clean 
					make -f ./MakeMyLib fclean
					$(RM) $(NAME)

re:						fclean all

.PHONY: all nolib withmylibstat clean fclean re