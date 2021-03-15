NAME	=	BombermanServer
CC	=	gcc -g -Wall -Wextra -Werror -std=c17
RM	=	rm -f
SRC	=	./src/server.c
OBJ	=	$(SRC:%.c=%.o)
LFLAGS	=	-L./lib -lmy_lib_$(shell uname -m)-$(shell uname -s)

all:					nolib

nolib:					$(OBJ)
					$(CC) ./main.c $(OBJ) -lpthread -Iinclude -o $(NAME)

clean:
					$(RM) $(OBJ)

fclean:					clean 
					$(RM) $(NAME)

re:						fclean all

.PHONY: all nolib clean fclean re