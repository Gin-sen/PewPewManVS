/*
** ETNA PROJECT, 04/10/2019 by places_m
** my_putchar
** File description:
**      [...]
*/
#ifndef unistd
#include <unistd.h>
#endif
void my_putchar(char c)
{
    write(1,&c,1);
}
