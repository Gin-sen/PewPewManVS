NAME	=	BombermanClient
CC	=	gcc -g -Wall -Wextra -Werror -std=c17
RM	=	rm -f
LFLAGS	=	-L./lib -lmy_lib_$(shell uname -m)-$(shell uname -s)

all:					nolib

nolib:
					$(CC) ./main.c -o $(NAME)

clean:
					$(RM) $(NAME)

re:						clean all

.PHONY: all nolib clean re